/**
 * intermediate_format/events/RawPingEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 99b49a30d83fefbc220c088c3d9f29cdcd06085daf655e7f11fcf33d00223e71
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawPingEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawPingEvent::RawPingEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) pinger)
  : RawFailableEvent(timestamp, index), pinger(move(pinger)) {
}

QString RawPingEvent::eventName() const {
    return "Ping";
}

void RawPingEvent::writeFailableEventDetailsToDebugStream(QDebug stream) const {
    stream << " pinger=" << pinger.get();
    if (pingee) {
        stream << " pingee=" << pingee.get();
    }
}

}}}
