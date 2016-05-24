/**
 * extraction/parse_libpurple_system_message.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>

#include "extraction/parse_libpurple_system_message.h"
#include "intermediate_format/events/RawCancelFileTransferEvent.h"
#include "intermediate_format/events/RawChangeScreenNameEvent.h"
#include "intermediate_format/events/RawOfferFileEvent.h"
#include "intermediate_format/events/RawReceiveFileEvent.h"
#include "intermediate_format/events/RawStartFileTransferEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/subjects/ImplicitSubject.h"
#include "intermediate_format/subjects/FullySpecifiedSubject.h"
#include "intermediate_format/subjects/SubjectGivenAsScreenName.h"
#include "protocols/parse_account_generic.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/html/entities.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::utils::html;

namespace uniarchive2 { namespace extraction {

CEDE(ApparentSubject) parse_subject(IMM(QString) subject, IMProtocol protocol);
QString parse_filename(IMM(QString) filename);


CEDE(RawEvent) parse_libpurple_system_message(
    unsigned int event_index,
    IMM(ApparentTime) event_time,
    IMM(QString) content,
    IMProtocol protocol
) {
    QREGEX_MATCH_CI(
        match,
        "^("\
        "((?<offer_who>.+) is offering to send file (?<offer_file>.+))|"\
        "(Starting transfer of (?<xfer_file>.+) from (?<xfer_from>.+))|"\
        "((?<cancel_who>.+) cancelled the transfer of (?<cancel_file>.+))|"\
        "(Transfer of file (?<recv_file>.+) complete)|"\
        "((?<rename_old>.+) is now known as (?<rename_new>.+)[.])|"\
        ")(<br/?>)?$",
        content.trimmed()
    );

    if (match.capturedLength("offer_who")) {
        return make_unique<RawOfferFileEvent>(
            event_time,
            event_index,
            parse_subject(match.captured("offer_who"), protocol),
            parse_filename(match.captured("offer_file"))
        );
    } else if (match.capturedLength("xfer_file")) {
        unique_ptr<RawEvent> xfer_event = make_unique<RawStartFileTransferEvent>(
            event_time,
            event_index,
            parse_filename(match.captured("xfer_file"))
        );
        static_cast<RawStartFileTransferEvent*>(xfer_event.get())->sender =
            parse_subject(match.captured("xfer_from"), protocol);

        return xfer_event;
    } else if (match.capturedLength("cancel_who")) {
        unique_ptr<RawEvent> xfer_event = make_unique<RawCancelFileTransferEvent>(
            event_time,
            event_index,
            parse_filename(match.captured("cancel_file"))
        );
        static_cast<RawCancelFileTransferEvent*>(xfer_event.get())->actor =
            parse_subject(match.captured("cancel_who"), protocol)

        return xfer_event;
    } else if (match.capturedLength("recv_file")) {
        return make_unique<RawReceiveFileEvent>(
            event_time,
            event_index,
            make_unique<ImplicitSubject>(ImplicitSubject::Kind::FILE_RECEIVER),
            parse_filename(match.captured("recv_file"))
        );
    } else if (match.capturedLength("rename_old")) {
        return make_unique<RawChangeScreenNameEvent>(
            event_time,
            event_index,
            parse_subject(match.captured("rename_old"), protocol),
            parse_subject(match.captured("rename_new"), protocol)
        );
    }

    invariant_violation("Unsupported libpurple system message: %s", QP(content));
}

CEDE(ApparentSubject) parse_subject(IMM(QString) subject, IMProtocol protocol) {
    QREGEX_MATCH_CI(match, "^(.*) \\[<em>(.*)</em>\\]$", subject);
    if (match.hasMatch()) {
        QString screen_name = decode_html_entities(match.captured(1));
        QString account_name = decode_html_entities(match.captured(2));

        QREGEX_MATCH_CI(strip_match, "^(.*@.*)/.*$", account_name);
        if (strip_match.hasMatch()) {
            account_name = strip_match.captured(1);
        }

        return make_unique<FullySpecifiedSubject>(
            parse_account_generic(protocol, account_name),
            screen_name
        );
    }

    return make_unique<SubjectGivenAsScreenName>(decode_html_entities(subject));
}

QString parse_filename(IMM(QString) filename) {
    QREGEX_MATCH_CI(link_match, "^<a href=\"(.*)\">.*</a>$", filename);

    if (link_match.hasMatch()) {
        return decode_html_entities(link_match.captured(1));
    }

    return decode_html_entities(filename);
}

}}
