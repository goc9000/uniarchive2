/**
 * intermediate_format/events/RawConnectedEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/RawConnectedEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawConnectedEvent::RawConnectedEvent(IMM(ApparentTime) timestamp, unsigned int index, TAKE(ApparentSubject) subject)
    : RawEvent(timestamp, index), subject(move(subject)) {
}

QString RawConnectedEvent::eventName() const {
    return "Connected";
}

void RawConnectedEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
}

}}}
