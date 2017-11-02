/**
 * extraction/pidgin/common_extract_pidgin_conversations_code.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "common_extract_pidgin_conversations_code.h"
#include "intermediate_format/provenance/ArchiveFileProvenance.h"
#include "intermediate_format/provenance/FileProvenance.h"
#include "intermediate_format/subjects/AccountSubject.h"
#include "intermediate_format/ApparentTime.h"
#include "protocols/FullAccountName.h"
#include "protocols/parse_account_generic.h"
#include "utils/time/parse_date_parts.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>
#include <QDir>
#include <QMap>

namespace uniarchive2 { namespace extraction { namespace pidgin {

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::provenance;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;
using namespace uniarchive2::utils::time;

struct InfoFromFilename {
    ApparentTime conversation_date;
    FullAccountName identity;
    optional<FullAccountName> peer;
    bool is_conference;
};

static InfoFromFilename analyze_conversation_filename(
   IMM(AtomicConversationSource) source,
   IMM(QString) expected_extension
);
static IMProtocol parse_protocol(IMM(QString) protocol_name);


RawConversation init_conversation(
    IMM(AtomicConversationSource) source,
    IMM(QString) expected_extension,
    ArchiveFormat format
) {
    auto info = analyze_conversation_filename(source, expected_extension);

    RawConversation conversation(info.identity.protocol);
    conversation.provenance = make_unique<ArchiveFileProvenance>(source.asProvenance(), format);

    conversation.declaredStartDate = info.conversation_date;
    conversation.isConference = info.is_conference;
    conversation.identity = make_unique<AccountSubject>(info.identity);
    if (info.peer) {
        conversation.declaredPeers.push_back(make_unique<AccountSubject>(*info.peer));
    }

    return conversation;
}

static InfoFromFilename analyze_conversation_filename(
    IMM(AtomicConversationSource) source,
    IMM(QString) expected_extension
) {
    InfoFromFilename info;

    QString full_filename = source.logicalFilename();
    QString protocol_folder = full_filename.section(QDir::separator(), -4, -4);
    QString identity_folder = full_filename.section(QDir::separator(), -3, -3);
    QString peer_folder = full_filename.section(QDir::separator(), -2, -2);
    QString base_name = source.baseName();
    QString extension = source.extension().toLower();

    invariant(
        extension == expected_extension,
        "Expected archive extension to be %s, but it is %s instead", QP(expected_extension), QP(extension)
    );
    QREGEX_MUST_MATCH_CI(
        filename_match, "^(?<date>\\d{4}-\\d{2}-\\d{2}[.]\\d{6})((?<offset>[+-]\\d+)(?<timezone>[a-z]+))?$", base_name,
        "Expected Pidgin archive filename to match pattern \"account_name (YYYY-mm-dd.hhmmss+offset/timezone)\", "\
        "found \"%s\""
    );

    QDateTime raw_date = QLocale::c().toDateTime(filename_match.captured("date"), "yyyy-MM-dd.hhmmss");

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

ApparentTime parse_timestamp(IMM(QString) timestamp_text, IMM(RawConversation) conversation) {
    ApparentTime timestamp;
    timestamp.reference = ApparentTimeReference::LOCAL_TIME;

    QREGEX_MUST_MATCH_CI(
        match,
        "((?<date_part>[^ ]+) )?(?<time_part>\\d+:\\d{2}:\\d{2}(?<ampm> (am|pm))?)",
        timestamp_text.trimmed(),
        "Could not find time part in \"%s\""
    );

    QTime time = QLocale::c().toTime(
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
            QDate candidate = QLocale::c().toDate(date_part, format);
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

}}}
