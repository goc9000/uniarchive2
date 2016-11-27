/**
 * intermediate_format/events/RawPingEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): d6fb9cc62b9b16e8ebe49e41eb73ef8b7c7e0afd5dd2ceb6f9b3d42e9da56ba0
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawPingEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawPingEvent::RawPingEvent(IMM(ApparentTime) timestamp, uint32_t index, TAKE(ApparentSubject) pinger)
  : RawFailableEvent(timestamp, index), pinger(move(pinger)) {
    sanityCheckMandatoryParameters();
}

void RawPingEvent::sanityCheckMandatoryParameters() const {
    invariant(pinger, "Parameter 'pinger' cannot have empty value");
}

RawEventSubType RawPingEvent::subType() const {
    return RawEventSubType::PING;
}

void RawPingEvent::serializeFailableEventDetailsToStream(QDataStream& mut_stream) const {
    // TODO: add field serialization code
}

void RawPingEvent::writeFailableEventDetailsToDebugStream(QDebug stream) const {
    stream << " pinger=" << pinger.get();
    if (pingee) {
        stream << " pingee=" << pingee.get();
    }
}

}}}
