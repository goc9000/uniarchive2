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
#include "intermediate_format/subjects/ImplicitSubject.h"
#include "intermediate_format/subjects/SubjectGivenAsScreenName.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::subjects;

namespace uniarchive2 { namespace extraction {

CEDE(RawEvent) parse_libpurple_system_message(
    unsigned int event_index,
    IMM(ApparentTime) event_time,
    IMM(QString) content
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
            make_unique<SubjectGivenAsScreenName>(match.captured("offer_who")),
            match.captured("offer_file")
        );
    } else if (match.capturedLength("xfer_file")) {
        unique_ptr<RawEvent> xfer_event =
            make_unique<RawStartFileTransferEvent>(event_time, event_index, match.captured("xfer_file"));
        static_cast<RawStartFileTransferEvent*>(xfer_event.get())->sender =
            make_unique<SubjectGivenAsScreenName>(match.captured("xfer_from"));

        return xfer_event;
    } else if (match.capturedLength("cancel_who")) {
        unique_ptr<RawEvent> xfer_event =
            make_unique<RawCancelFileTransferEvent>(event_time, event_index, match.captured("cancel_file"));
        static_cast<RawCancelFileTransferEvent*>(xfer_event.get())->actor =
            make_unique<SubjectGivenAsScreenName>(match.captured("cancel_who"));

        return xfer_event;
    } else if (match.capturedLength("recv_file")) {
        return make_unique<RawReceiveFileEvent>(
            event_time,
            event_index,
            make_unique<ImplicitSubject>(ImplicitSubject::Kind::FILE_RECEIVER),
            match.captured("recv_file")
        );
    } else if (match.capturedLength("rename_old")) {
        return make_unique<RawChangeScreenNameEvent>(
            event_time,
            event_index,
            make_unique<SubjectGivenAsScreenName>(match.captured("rename_old")),
            make_unique<SubjectGivenAsScreenName>(match.captured("rename_new"))
        );
    }

    invariant_violation("Unsupported libpurple system message: %s", QP(content));
}

}}
