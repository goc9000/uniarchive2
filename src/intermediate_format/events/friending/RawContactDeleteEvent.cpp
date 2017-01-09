/**
 * intermediate_format/events/friending/RawContactDeleteEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): f1082aea32ec7dd8f85468d9acb4dc9b6c26a1b5afb3e717a919493840a27938
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/friending/RawContactDeleteEvent.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawContactDeleteEvent::RawContactDeleteEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) deleter,
    TAKE(ApparentSubject) deletee
) : RawEvent(timestamp, index), deleter(move(deleter)), deletee(move(deletee)) {
    sanityCheckMandatoryParameters();
}

void RawContactDeleteEvent::sanityCheckMandatoryParameters() const {
    invariant(deleter, "Parameter 'deleter' cannot have empty value");
    invariant(deletee, "Parameter 'deletee' cannot have empty value");
}

RawEventSubType RawContactDeleteEvent::subType() const {
    return RawEventSubType::CONTACT_DELETE;
}

CEDE(RawContactDeleteEvent) RawContactDeleteEvent::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    invariant_violation("RawContactDeleteEvent deserialization not yet implemented");
}

void RawContactDeleteEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << deleter << deletee;
}

void RawContactDeleteEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " deleter=" << deleter << " deletee=" << deletee;
}

}}}
