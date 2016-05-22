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

void parse_message(IMM(QRegularExpressionMatch) event_match, IMM(RawConversation) conversation) {
    QString color = event_match.captured("color");

    if (event_match.capturedLength("system_message")) {
        invariant(color == "" || color == "#FF0000", "Expected color to be absent or #FF0000 for system message");

        // TODO: parse system message
    } else {
        invariant(
            color == "#16569E" || color == "#A82F2F",
            "Expected color to be #16569E or #A82F2F for system message"
        );

        bool is_self = (color == "#16569E");

        // TODO: parse regular message
    }
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

}}}