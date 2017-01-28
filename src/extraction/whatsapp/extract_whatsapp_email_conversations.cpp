/**
 * extraction/whatsapp/extract_whatsapp_email_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "extraction/whatsapp/extract_whatsapp_email_conversations.h"
#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/content/MediaAttachment.h"
#include "intermediate_format/events/conference/RawAddToConferenceEvent.h"
#include "intermediate_format/events/conference/RawCreateConferenceEvent.h"
#include "intermediate_format/events/system/RawEEEncryptionAnnouncementEvent.h"
#include "intermediate_format/events/RawMessageEvent.h"
#include "intermediate_format/provenance/ArchiveFileProvenance.h"
#include "intermediate_format/subjects/ImplicitSubject.h"
#include "intermediate_format/subjects/ScreenNameSubject.h"
#include "protocols/ArchiveFormat.h"
#include "protocols/IMProtocol.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/text/load_text_file.h"
#include "utils/text/split_into_lines.h"

#include <QtDebug>
#include <QDir>
#include <QFileInfo>
#include <QLocale>

namespace uniarchive2 { namespace extraction { namespace whatsapp {

using namespace std;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::provenance;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;
using namespace uniarchive2::utils::text;

struct PreParsedEvent {
    QString raw_timestamp;
    QString raw_screen_name;
    QString raw_content;
};

static RawConversation init_conversation(IMM(QString) filename);
static vector<PreParsedEvent> pre_parse_events(IMM(QString) text_data);
static CEDE(RawEvent) parse_event(IMM(PreParsedEvent) raw_event, uint32_t event_index);
static ApparentTime parse_timestamp(IMM(QString) time_text);
static CEDE(RawEvent) parse_system_event(IMM(ApparentTime) timestamp, uint32_t event_index, IMM(QString) content);
static CEDE(ApparentSubject) parse_subject(IMM(QString) raw_subject);
static CEDE(RawEvent) parse_message(
    IMM(ApparentTime) timestamp,
    uint32_t event_index,
    IMM(QString) raw_subject,
    IMM(QString) content
);


RawConversation extract_whatsapp_email_conversation(IMM(QString)filename) {
    RawConversation conversation = init_conversation(filename);

    for (IMM(auto) raw_event : pre_parse_events(load_utf8_text_file(filename))) {
        unique_ptr<RawEvent> event = parse_event(raw_event, (uint)conversation.events.size());

        if (event->is<RawCreateConferenceEvent>()) {
            conversation.isConference = true;
            conversation.declaredPeers.clear();
            conversation.declaredPeers.push_back(event->as<RawCreateConferenceEvent>()->creator->clone());
            conversation.conferenceTitle = event->as<RawCreateConferenceEvent>()->conferenceName;
        }

        conversation.events.push_back(move(event));
    }

    return conversation;
}

static RawConversation init_conversation(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", QP(filename));

    QString full_filename = file_info.absoluteFilePath();

    QREGEX_MUST_MATCH(
        name_match, "^WhatsApp Chat with (.+)[.]txt$", full_filename.section(QDir::separator(), -1, -1),
        "WhatApp conversation filename should be 'WhatsApp Chat with Peer Name.txt', is instead '%s'"
    );

    RawConversation conversation(IMProtocol::WHATSAPP);
    conversation.provenance = ArchiveFileProvenance::fromQFileInfo(ArchiveFormat::WHATSAPP_EMAIL, file_info);

    // Note that this may be corrected later
    conversation.isConference = false;
    conversation.declaredPeers.push_back(parse_subject(name_match.captured(1)));

    return conversation;
}

vector<PreParsedEvent> pre_parse_events(IMM(QString) text_data) {
    vector<PreParsedEvent> raw_events;

    QStringList lines = split_into_lines(text_data);

    PreParsedEvent raw_event = PreParsedEvent();

    for (int i = 0; i < lines.length(); i++) {
        QREGEX_MATCH(
            start_match,
            "^(?<timestamp>[^,]*, \\d+:\\d+) - ((?<screen_name>[^:]+): )?(?<content>.*)$",
            lines[i]
        );

        if (!start_match.hasMatch()) {
            invariant(i != 0, "First event in file does not match format!");
            raw_event.raw_content.append("\n").append(lines[i].trimmed());
        } else {
            if (i != 0) {
                raw_events.push_back(raw_event);
            }
            raw_event = PreParsedEvent();
            raw_event.raw_timestamp = start_match.captured("timestamp");
            raw_event.raw_screen_name = start_match.captured("screen_name");
            raw_event.raw_content = start_match.captured("content");
        }

        if (i == lines.length() - 1) {
            raw_events.push_back(raw_event);
        }
    }

    return raw_events;
}

static CEDE(RawEvent) parse_event(IMM(PreParsedEvent) raw_event, uint32_t event_index) {
    ApparentTime timestamp = parse_timestamp(raw_event.raw_timestamp);

    return raw_event.raw_screen_name.isEmpty()
       ? parse_system_event(timestamp, event_index, raw_event.raw_content)
       : parse_message(timestamp, event_index, raw_event.raw_screen_name, raw_event.raw_content);
}

static ApparentTime parse_timestamp(IMM(QString) time_text) {
    QDateTime datetime = QLocale::c().toDateTime(time_text, "dd/MM/yyyy, hh:mm");
    invariant(datetime.isValid(), "Invalid timestamp format: %s", QP(time_text));

    ApparentTime timestamp = ApparentTime::fromQDateTimeLocalTime(datetime);
    timestamp.secondsSpecified = false;

    return timestamp;
}

static CEDE(RawEvent) parse_system_event(IMM(ApparentTime) timestamp, uint32_t event_index, IMM(QString) content) {
    QREGEX_MATCH_CI(
        match,
        "^("\
        "((?<grp_create_who>.+) created group .(?<grp_create_name>.+).)|"\
        "((?<grp_add_you>.+) added you)|"\
        "(?<ee_note>Messages you send to this chat and calls are now secured with end-to-end encryption.*)"\
        ")$",
        content
    );

    if (match.capturedLength("ee_note")) {
        return make_unique<RawEEEncryptionAnnouncementEvent>(timestamp, event_index);
    } else if (match.capturedLength("grp_create_who")) {
        return make_unique<RawCreateConferenceEvent>(
            timestamp,
            event_index,
            parse_subject(match.captured("grp_create_who")),
            match.captured("grp_create_name")
        );
    } else if (match.capturedLength("grp_add_you")) {
        return make_unique<RawAddToConferenceEvent>(
            timestamp,
            event_index,
            parse_subject(match.captured("grp_add_you")),
            make_unique<ImplicitSubject>(ImplicitSubjectKind::IDENTITY)
        );
    }

    invariant_violation("Unsupported WhatsApp system message: %s", QP(content));
}

static CEDE(ApparentSubject) parse_subject(IMM(QString) raw_subject) {
    return make_unique<ScreenNameSubject>(raw_subject);
}

static CEDE(RawEvent) parse_message(
    IMM(ApparentTime) timestamp,
    uint32_t event_index,
    IMM(QString) raw_subject,
    IMM(QString) text_content
) {
    RawMessageContent content;

    QREGEX_MATCH_CI(
        match,
        "^("\
        "((?<filename>.+[.].+) \\(file attached\\))|"\
        "(?<omitted><Media omitted>)"\
        ")$",
        text_content
    );

    if (match.capturedLength("filename")) {
        unique_ptr<RawMessageContentItem> media = make_unique<MediaAttachment>();
        media->as<MediaAttachment>()->filename = match.captured("filename");
        content.addItem(move(media));
    } else if (match.capturedLength("omitted")) {
        unique_ptr<RawMessageContentItem> media = make_unique<MediaAttachment>();
        media->as<MediaAttachment>()->error = MediaError::OMITTED;
        content.addItem(move(media));
    } else {
        content = RawMessageContent::fromPlainText(text_content);
    }

    return make_unique<RawMessageEvent>(
        timestamp,
        event_index,
        parse_subject(raw_subject),
        move(content)
    );
}

}}}
