/**
 * intermediate_format/events/conference/RawLeaveConferenceEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawLeaveConferenceEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawLeaveConferenceEvent::RawLeaveConferenceEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) subject)
  : RawEvent(timestamp, index), subject(move(subject)) {
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
