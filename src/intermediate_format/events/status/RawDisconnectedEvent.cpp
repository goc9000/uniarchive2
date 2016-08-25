/**
 * intermediate_format/events/status/RawDisconnectedEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/status/RawDisconnectedEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawDisconnectedEvent::RawDisconnectedEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) subject)
    : RawEvent(timestamp, index), subject(move(subject)) {
}

QString RawDisconnectedEvent::eventName() const {
    return "Disconnected";
}

void RawDisconnectedEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
}

}}}
