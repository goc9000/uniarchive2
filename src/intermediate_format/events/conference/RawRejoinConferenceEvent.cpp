/**
 * intermediate_format/events/conference/RawRejoinConferenceEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 4663be966d78a57fd82689484d32b7cec98f1efe6f626374f6cff9c54c060582
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawRejoinConferenceEvent.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawRejoinConferenceEvent::RawRejoinConferenceEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) subject,
    TAKE_VEC(ApparentSubject) peers
) : RawEvent(timestamp, index), subject(move(subject)), peers(move(peers)) {
    sanityCheckMandatoryParameters();
}

void RawRejoinConferenceEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
    for (IMM(auto) item : peers) {
        invariant(item, "Parameter 'peers' cannot have empty entries");
    }
}

RawEventSubType RawRejoinConferenceEvent::subType() const {
    return RawEventSubType::REJOIN_CONFERENCE;
}

CEDE(RawRejoinConferenceEvent) RawRejoinConferenceEvent::deserializeFromStream(
    QDataStream& mut_stream,
    bool skip_type
) {
    invariant_violation("RawRejoinConferenceEvent deserialization not yet implemented");
}

void RawRejoinConferenceEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << subject << peers;
}

void RawRejoinConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject << " peers=" << peers;
}

}}}
