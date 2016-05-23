/**
 * extraction/pidgin/extract_pidgin_conversations.cpp
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
#include <QIODevice>
#include <QTextCodec>
#include <QTimeZone>

#include "extraction/pidgin/extract_pidgin_conversations.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "intermediate_format/subjects/SubjectGivenAsScreenName.h"
#include "protocols/FullAccountName.h"
#include "protocols/IMProtocol.h"
#include "protocols/IMStatus.h"
#include "protocols/parse_account_generic.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/external_libs/optional.hpp"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/time/parse_date_parts.h"

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;
using namespace uniarchive2::utils::time;

namespace uniarchive2 { namespace extraction { namespace pidgin {

struct InfoFromFilename {
    ApparentTime conversation_date;
    FullAccountName identity;
    optional<FullAccountName> peer;
    bool is_conference;
};

RawConversation init_conversation(IMM(QString)filename);
InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename);
IMProtocol parse_protocol(IMM(QString) protocol_name);

void verify_is_utf8_html(QTextStream& mut_stream);
void seek_to_start_of_events(QTextStream& mut_stream);

void parse_message(IMM(QRegularExpressionMatch) event_match, IMM(RawConversation) conversation);
ApparentTime parse_timestamp(IMM(QString) timestamp_text, IMM(RawConversation) conversation);
QString strip_sender_suffix(IMM(QString) sender);


RawConversation extract_pidgin_html_conversation(IMM(QString) filename) {
    RawConversation conversation = init_conversation(filename);

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal("Can't open file: %s", QP(filename));
    }

    // Use regular expressions because newlines are very significant in parsing here
    QTextStream stream(&file);
    verify_is_utf8_html(stream);
    seek_to_start_of_events(stream);

    QString full_text = stream.readAll();

    QREGEX_CI(
        event_pattern,
        "("\
          "(<font color=\"(?<color>#[a-z0-9]{6})\">)?" \
          "<font size=\"2\">\\((?<timestamp>[^)]*)\\)</font>" \
          "(" \
            "( <b>(?<sender>[^:]*):</b></font> (?<message>.*?))|" \
            "(<b> (?<system_message>.*?)</b>(</font>)?)" \
          ")" \
          "<br/>\r?\n" \
        ")|("\
          "?<end_of_file></body></html>\\s*$"\
        ")"
    );

    int offset = 0;
    while (offset < full_text.length()) {
        auto event_match = event_pattern.match(full_text, offset);
        invariant(
            event_match.hasMatch() && (event_match.capturedStart(0) == offset),
            "Failed to parse event at text: \"%s...\"", QP(full_text.mid(offset, 256))
        );
        if (event_match.capturedLength("end_of_file")) {
            break;
        }

        parse_message(event_match, conversation);

        offset = event_match.capturedEnd(0);
    }

    return conversation;
}

RawConversation init_conversation(IMM(QString)filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", QP(filename));

    QString full_filename = file_info.absoluteFilePath();
    auto info = analyze_conversation_filename(full_filename);

    RawConversation conversation(ArchiveFormat::PIDGIN_HTML, info.identity.protocol);

    conversation.originalFilename = full_filename;
    conversation.fileLastModifiedTime = ApparentTime::fromQDateTimeUnknownReference(file_info.lastModified());
    conversation.declaredStartDate = info.conversation_date;
    conversation.isConference = info.is_conference;
    conversation.identity = make_unique<SubjectGivenAsAccount>(info.identity);
    if (info.peer) {
        conversation.declaredPeers.push_back(make_unique<SubjectGivenAsAccount>(*info.peer));
    }

    return conversation;
}

InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename) {
    InfoFromFilename info;

    QString protocol_folder = full_filename.section(QDir::separator(), -4, -4);
    QString identity_folder = full_filename.section(QDir::separator(), -3, -3);
    QString peer_folder = full_filename.section(QDir::separator(), -2, -2);
    QString base_name = full_filename.section(QDir::separator(), -1, -1);

    QREGEX_MUST_MATCH_CI(
        filename_match, "^(\\d{4}-\\d{2}-\\d{2}[.]\\d{6})([+-]\\d+)([a-z]+)[.]html$", base_name,
        "Expected Pidgin archive filename to match pattern \"account_name (YYYY-mm-dd.hhmmss+offset/timezone).html\", "\
        "found \"%s\""
    );

    QDateTime raw_date = QDateTime::fromString(filename_match.captured(1), "yyyy-MM-dd.hhmmss");
    int offset_quarters = parse_timezone_offset_in_quarters(filename_match.captured(2));

    raw_date.setTimeSpec(Qt::OffsetFromUTC);
    raw_date.setOffsetFromUtc(900 * offset_quarters);

    info.conversation_date = ApparentTime::fromQDateTime(raw_date);
    info.conversation_date.timeZoneAbbreviation = filename_match.captured(3);

    IMProtocol protocol = parse_protocol(protocol_folder);
    info.identity = parse_account_generic(protocol, identity_folder);

    if (peer_folder.endsWith(".chat")) {
        info.is_conference = true;
    } else {
        info.peer = parse_account_generic(protocol, peer_folder);
        info.is_conference = false;
    }

    return info;
}

IMProtocol parse_protocol(IMM(QString) protocol_name) {
    const static QMap<QString, IMProtocol> PROTOCOL_MAP = {
        { "jabber", IMProtocol::JABBER },
        { "msn"   , IMProtocol::MSN },
        { "yahoo" , IMProtocol::YAHOO },
    };

    if (PROTOCOL_MAP.contains(protocol_name)) {
        return PROTOCOL_MAP[protocol_name];
    }

    invariant_violation("Unrecognized protocol in Pidgin: \"%s\"", QP(protocol_name));
}

void verify_is_utf8_html(QTextStream& mut_stream) {
    QByteArray expected_header("<html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">");
    QByteArray actual_header = mut_stream.device()->peek(expected_header.length());

    invariant(
         actual_header == expected_header,
         "Expected header to be \"\", found \"\"", qPrintable(expected_header), qPrintable(actual_header)
    );

    mut_stream.setCodec(QTextCodec::codecForName("UTF-8"));
}

void seek_to_start_of_events(QTextStream& mut_stream) {
    QString first_line = mut_stream.readLine();

    QREGEX_MUST_MATCH_CI(
        match,
        "^<html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">"\
        "<title>Conversation with (.*) at (.*) on (.*) \\((.*)\\)</title>"\
        "</head><body><h3>Conversation with (.*) at (.*) on (.*) \\((.*)\\)</h3>$",
        first_line,
        "Unexpected format for first line of archive: \"%s\""
    );
}

void parse_message(IMM(QRegularExpressionMatch) event_match, IMM(RawConversation) conversation) {
    QString color = event_match.captured("color");
    ApparentTime timestamp = parse_timestamp(event_match.captured("timestamp"), conversation);

    if (event_match.capturedLength("system_message")) {
        invariant(color == "" || color == "#FF0000", "Expected color to be absent or #FF0000 for system message");

        // TODO: parse system message
    } else {
        invariant(
            color == "#16569E" || color == "#A82F2F",
            "Expected color to be #16569E or #A82F2F for system message"
        );

        bool is_self = (color == "#16569E");
        unique_ptr<ApparentSubject> sender = make_unique<SubjectGivenAsScreenName>(
            strip_sender_suffix(event_match.captured("sender")),
            is_self ? ApparentSubject::Hint::IsIdentity : ApparentSubject::Hint::IsPeer
        );

        // TODO: parse regular message
    }
}

ApparentTime parse_timestamp(IMM(QString) timestamp_text, IMM(RawConversation) conversation) {
    ApparentTime timestamp;
    timestamp.reference = ApparentTime::Reference::LOCAL_TIME;

    QREGEX_MUST_MATCH_CI(
        match,
        "((?<date_part>[^ ]+) )?(?<time_part>\\d+:\\d{2}:\\d{2}(?<ampm> (am|pm))?)",
        timestamp_text.trimmed(),
        "Could not find time part in \"%s\""
    );

    QTime time = QTime::fromString(
        match.captured("time_part").toLower(),
        match.capturedLength("ampm") ? "h:mm:ss ap" : "h:mm:ss"
    );
    invariant(time.isValid(), "Invalid time: \"%s\"", QP(match.captured("time_part")));

    timestamp.time = time;

    QString date_part = match.captured("date_part").trimmed();
    if (!date_part.isEmpty()) {
        // Now the nightmare starts. Date parts could be in any order, with any separator...
        QREGEX(separator, "\\W");
        date_part = date_part.replace(separator, "-");

        vector<QDate> candidates;
        QStringList date_formats { "d-M-yyyy", "M-d-yyyy", "yyyy-M-d", "yyyy-d-M" };

        for (IMM(QString) format : date_formats) {
            QDate candidate = QDate::fromString(date_part, format);
            if (candidate.isValid()) {
                candidates.push_back(candidate);
            }
        }

        invariant(candidates.size() > 0, "Could not parse date \"%s\" with any format", QP(date_part));

        // Disambiguate using the closest date to the conversation date
        QDate convo_date = *(conversation.declaredStartDate->date);
        sort(
            candidates.begin(), candidates.end(),
            [convo_date](IMM(QDate) date_a, IMM(QDate) date_b) -> bool {
                auto delta_a = abs((int)date_a.daysTo(convo_date));
                auto delta_b = abs((int)date_b.daysTo(convo_date));
                return delta_a < delta_b;
            }
        );

        timestamp.date = candidates.front();
    }

    return timestamp;
}

QString strip_sender_suffix(IMM(QString) sender) {
    QREGEX_MATCH_CI(match, "(.*@[^/]*)/.*", sender);
    return match.hasMatch() ? match.captured(1) : sender;
}

}}}