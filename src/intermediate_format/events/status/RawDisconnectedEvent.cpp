/**
 * intermediate_format/events/status/RawDisconnectedEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): bf5959e1429fb337b57bedf2d3ade075f236dceb749f4c1f1cf314d04250ca87
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/status/RawDisconnectedEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawDisconnectedEvent::RawDisconnectedEvent(IMM(ApparentTime) timestamp, uint32_t index, TAKE(ApparentSubject) subject)
  : RawEvent(timestamp, index), subject(move(subject)) {
    sanityCheckMandatoryParameters();
}

void RawDisconnectedEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
}

RawEventSubType RawDisconnectedEvent::subType() const {
    return RawEventSubType::DISCONNECTED;
}

void RawDisconnectedEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    // TODO: add field serialization code
}

void RawDisconnectedEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
}

}}}
