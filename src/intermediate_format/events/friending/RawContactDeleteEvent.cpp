/**
 * intermediate_format/events/friending/RawContactDeleteEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 39398d4b69c06f7f19a0b06cb711128c97216fedd6ecb4e1f81a81742e5c5467
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/friending/RawContactDeleteEvent.h"
#include "utils/qt/debug_extras.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

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

void RawContactDeleteEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    // TODO: add field serialization code
}

void RawContactDeleteEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " deleter=" << deleter << " deletee=" << deletee;
}

}}}
