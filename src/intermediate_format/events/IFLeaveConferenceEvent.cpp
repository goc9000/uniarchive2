/**
 * intermediate_format/events/IFLeaveConferenceEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFLeaveConferenceEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFLeaveConferenceEvent::IFLeaveConferenceEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    unique_ptr<ApparentSubject> subject
): IntermediateFormatEvent(timestamp, index), subject(move(subject)) {
}

QString IFLeaveConferenceEvent::eventName() const {
    return "LeaveConference";
}

void IFLeaveConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
    if (message) {
        stream << " message=" << *message;
    }
}

}}}
