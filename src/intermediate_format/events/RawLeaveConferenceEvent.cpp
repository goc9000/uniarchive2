/**
 * intermediate_format/events/RawLeaveConferenceEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/RawLeaveConferenceEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawLeaveConferenceEvent::RawLeaveConferenceEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) subject
): RawEvent(timestamp, index), subject(move(subject)) {
}

QString RawLeaveConferenceEvent::eventName() const {
    return "LeaveConference";
}

void RawLeaveConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
    if (message) {
        stream << " message=" << *message;
    }
}

}}}
