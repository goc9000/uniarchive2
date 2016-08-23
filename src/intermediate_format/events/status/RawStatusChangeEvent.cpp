/**
 * intermediate_format/events/status/RawStatusChangeEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/status/RawStatusChangeEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawStatusChangeEvent::RawStatusChangeEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) subject,
    IMStatus status
): RawEvent(timestamp, index), subject(move(subject)), status(status) {
}

QString RawStatusChangeEvent::eventName() const {
    return "StatusChange";
}

void RawStatusChangeEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get() << " status=" << status;
    if (message) {
        stream << " message=" << *message;
    }
}

}}}
