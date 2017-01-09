/**
 * intermediate_format/events/friending/RawContactRequestEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 2ac6c75eeb8c58cd1a10184af15f16f482ecdad2ad4821a52cdb2a711c735184
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/friending/RawContactRequestEvent.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawContactRequestEvent::RawContactRequestEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) sender,
    TAKE(ApparentSubject) receiver,
    RawMessageContent&& message
) : RawEvent(timestamp, index), sender(move(sender)), receiver(move(receiver)), message(move(message)) {
    sanityCheckMandatoryParameters();
}

void RawContactRequestEvent::sanityCheckMandatoryParameters() const {
    invariant(sender, "Parameter 'sender' cannot have empty value");
    invariant(receiver, "Parameter 'receiver' cannot have empty value");
}

RawEventSubType RawContactRequestEvent::subType() const {
    return RawEventSubType::CONTACT_REQUEST;
}

CEDE(RawContactRequestEvent) RawContactRequestEvent::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    invariant_violation("RawContactRequestEvent deserialization not yet implemented");
}

void RawContactRequestEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << sender << receiver << message;
}

void RawContactRequestEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender << " receiver=" << receiver << " message=" << message;
}

}}}
