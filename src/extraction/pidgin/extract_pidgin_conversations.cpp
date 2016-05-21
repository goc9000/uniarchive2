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
#include <QTimeZone>

#include "extraction/pidgin/extract_pidgin_conversations.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "protocols/FullAccountName.h"
#include "protocols/IMProtocol.h"
#include "protocols/IMStatus.h"
#include "protocols/parse_account_generic.h"
#include "utils/external_libs/make_unique.hpp"
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
};

RawConversation init_conversation(IMM(QString)filename);
InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename);
IMProtocol parse_protocol(IMM(QString) protocol_name);


RawConversation extract_pidgin_html_conversation(IMM(QString)filename) {
    RawConversation conversation = init_conversation(filename);

    return conversation;
}

RawConversation init_conversation(IMM(QString)filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", QP(filename));

    QString full_filename = file_info.absoluteFilePath();
    auto info = analyze_conversation_filename(full_filename);

    RawConversation conversation(ArchiveFormat::PIDGIN_HTML, IMProtocol::INVALID);

    conversation.originalFilename = full_filename;
    conversation.fileLastModifiedTime = ApparentTime::fromQDateTimeUnknownReference(file_info.lastModified());
    conversation.declaredStartDate = info.conversation_date;

    return conversation;
}

InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename) {
    InfoFromFilename info;

    QString protocol_folder = full_filename.section(QDir::separator(), -4, -4);
    QString identity_folder = full_filename.section(QDir::separator(), -3, -3);
    QString peer_folder = full_filename.section(QDir::separator(), -2, -2);
    QString base_name = full_filename.section(QDir::separator(), -1, -1);

    QREGEX_CI(filename_pattern, "^(\\d{4}-\\d{2}-\\d{2}[.]\\d{6})([+-]\\d+)([a-z]+)[.]html$");

    auto filename_match = filename_pattern.match(base_name);
    invariant(
        filename_match.hasMatch(),
        "Expected Pidgin archive filename to match pattern \"account_name (YYYY-mm-dd.hhmmss+offset/timezone).html\", "
            "found \"%s\"",
        qUtf8Printable(base_name)
    );

    QDateTime raw_date = QDateTime::fromString(filename_match.captured(1), "yyyy-MM-dd.hhmmss");
    int offset_quarters = parse_timezone_offset_in_quarters(filename_match.captured(2));

    raw_date.setTimeSpec(Qt::OffsetFromUTC);
    raw_date.setOffsetFromUtc(900 * offset_quarters);

    info.conversation_date = ApparentTime::fromQDateTime(raw_date);
    info.conversation_date.timeZoneAbbreviation = filename_match.captured(3);

    IMProtocol protocol = parse_protocol(protocol_folder);

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

}}}