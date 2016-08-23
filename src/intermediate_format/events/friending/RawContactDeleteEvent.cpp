/**
 * intermediate_format/events/friending/RawContactDeleteEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/friending/RawContactDeleteEvent.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawContactDeleteEvent::RawContactDeleteEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) deleter,
    TAKE(ApparentSubject) deletee
): RawEvent(timestamp, index), deleter(move(deleter)), deletee(move(deletee)) {
}

QString RawContactDeleteEvent::eventName() const {
    return "ContactDelete";
}

void RawContactDeleteEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " deleter=" << deleter.get() << " deletee=" << deletee.get();
}

}}}
