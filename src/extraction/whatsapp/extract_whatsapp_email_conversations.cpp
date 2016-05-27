/**
 * extraction/whatsapp/extract_whatsapp_email_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>
#include <QDir>
#include <QFileInfo>

#include "extraction/whatsapp/extract_whatsapp_email_conversations.h"
#include "protocols/ArchiveFormat.h"
#include "protocols/IMProtocol.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/text/load_text_file.h"
#include "utils/text/split_into_lines.h"

using namespace std;
using namespace uniarchive2::protocols;
using namespace uniarchive2::utils::text;

namespace uniarchive2 { namespace extraction { namespace whatsapp {

struct PreParsedEvent {
    QString raw_timestamp;
    QString raw_screen_name;
    QString raw_content;
};

static RawConversation init_conversation(IMM(QString) filename);
vector<PreParsedEvent> pre_parse_events(IMM(QString) text_data);


RawConversation extract_whatsapp_email_conversation(IMM(QString)filename) {
    RawConversation conversation = init_conversation(filename);

    for (IMM(auto) raw_event : pre_parse_events(load_utf8_text_file(filename))) {
    }

    return conversation;
}

static RawConversation init_conversation(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", QP(filename));

    QString full_filename = file_info.absoluteFilePath();

    QREGEX_MUST_MATCH(
        name_match, "^WhatsApp Chat with .+[.]txt$", full_filename.section(QDir::separator(), -1, -1),
        "WhatApp conversation filename should be 'WhatSapp Chat with Peer Name.txt', is instead '%s'"
    );

    RawConversation conversation(ArchiveFormat::WHATSAPP_EMAIL, IMProtocol::WHATSAPP);
    conversation.originalFilename = full_filename;
    conversation.fileLastModifiedTime = ApparentTime::fromQDateTimeUnknownReference(file_info.lastModified());

    return conversation;
}

vector<PreParsedEvent> pre_parse_events(IMM(QString) text_data) {
    vector<PreParsedEvent> raw_events;

    QStringList lines = split_into_lines(text_data);

    PreParsedEvent raw_event = PreParsedEvent();

    for (int i = 0; i < lines.length(); i++) {
        QREGEX_MATCH(
            start_match,
            "^"\
            "(?<timestamp>[^,]*, \\d+:\\d+)"\
            "( - (?<screen_name>[^:]+): )?"\
            "(?<content>.*)"\
            "$",
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

}}}