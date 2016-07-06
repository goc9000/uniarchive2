/**
 * intermediate_format/events/conference/RawAddToConferenceEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawAddToConferenceEvent.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawAddToConferenceEvent::RawAddToConferenceEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) adder,
    TAKE(ApparentSubject) addee
): RawEvent(timestamp, index), adder(move(adder)), addee(move(addee)) {
}

RawAddToConferenceEvent::RawAddToConferenceEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) addee
): RawEvent(timestamp, index), addee(move(addee)) {
}

QString RawAddToConferenceEvent::eventName() const {
    return "AddToConference";
}

void RawAddToConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (adder) {
        stream << " adder=" << adder.get();
    }
    stream << " addee=" << addee.get();
}

}}}
