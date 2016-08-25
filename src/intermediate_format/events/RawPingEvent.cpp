/**
 * intermediate_format/events/RawPingEvent.cpp
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
