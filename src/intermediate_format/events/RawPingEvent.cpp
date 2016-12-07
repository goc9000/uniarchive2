/**
 * intermediate_format/events/RawPingEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 28fc8d6d5be8c234cc2618ed303314ed5bcd05c7a4ffc06948caa6aecd4ab828
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawPingEvent.h"
#include "utils/qt/debug_extras.h"

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
    stream << " pinger=" << pinger;
    if (pingee) {
        stream << " pingee=" << pingee;
    }
}

}}}
