/**
 * extraction/pidgin/common_extract_pidgin_conversations_code.cpp
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
#include <QMap>

#include "common_extract_pidgin_conversations_code.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "intermediate_format/ApparentTime.h"
#include "protocols/FullAccountName.h"
#include "protocols/parse_account_generic.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/time/parse_date_parts.h"
#include "utils/qt/shortcuts.h"

using namespace std;
using namespace uniarchive2::intermediate_format;
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

static InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename, IMM(QString) expected_extension);
static IMProtocol parse_protocol(IMM(QString) protocol_name);


RawConversation init_conversation(IMM(QString)filename, IMM(QString) expected_extension, ArchiveFormat format) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", QP(filename));

    QString full_filename = file_info.absoluteFilePath();
    auto info = analyze_conversation_filename(full_filename, expected_extension);

    RawConversation conversation(format, info.identity.protocol);

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

static InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename, IMM(QString) expected_extension) {
    InfoFromFilename info;

    QFileInfo file_info(full_filename);
    QString protocol_folder = full_filename.section(QDir::separator(), -4, -4);
    QString identity_folder = full_filename.section(QDir::separator(), -3, -3);
    QString peer_folder = full_filename.section(QDir::separator(), -2, -2);
    QString base_name = file_info.completeBaseName();
    QString extension = file_info.suffix().toLower();

    invariant(
        extension == expected_extension,
        "Expected archive extension to be %s, but it is %s instead", QP(expected_extension), QP(extension)
    );
    QREGEX_MUST_MATCH_CI(
        filename_match, "^(?<date>\\d{4}-\\d{2}-\\d{2}[.]\\d{6})((?<offset>[+-]\\d+)(?<timezone>[a-z]+))?$", base_name,
        "Expected Pidgin archive filename to match pattern \"account_name (YYYY-mm-dd.hhmmss+offset/timezone)\", "\
        "found \"%s\""
    );

    QDateTime raw_date = QDateTime::fromString(filename_match.captured("date"), "yyyy-MM-dd.hhmmss");

    if (filename_match.capturedLength("offset")) {
        int offset_quarters = parse_timezone_offset_in_quarters(filename_match.captured("offset"));
        raw_date.setTimeSpec(Qt::OffsetFromUTC);
        raw_date.setOffsetFromUtc(900 * offset_quarters);

        info.conversation_date = ApparentTime::fromQDateTime(raw_date);
        info.conversation_date.timeZoneAbbreviation = filename_match.captured("timezone");
    } else {
        info.conversation_date = ApparentTime::fromQDateTimeLocalTime(raw_date);
    }

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

static IMProtocol parse_protocol(IMM(QString) protocol_name) {
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