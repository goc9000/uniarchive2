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

#include "utils/qt/debug_extras.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawAddToConferenceEvent::RawAddToConferenceEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) adder,
    TAKE_VEC(ApparentSubject) addees
): RawEvent(timestamp, index), adder(move(adder)), addees(move(addees)) {
}

RawAddToConferenceEvent::RawAddToConferenceEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) adder,
    TAKE(ApparentSubject) addee
): RawEvent(timestamp, index), adder(move(adder)) {
    addees.push_back(move(addee));
}

RawAddToConferenceEvent::RawAddToConferenceEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) addee)
    : RawEvent(timestamp, index) {
    addees.push_back(move(addee));
}

QString RawAddToConferenceEvent::eventName() const {
    return "AddToConference";
}

void RawAddToConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (adder) {
        stream << " adder=" << adder.get();
    }
    if (addees.size() == 1) {
        stream << " addee=" << addees.front();
    } else {
        stream << " addees=" << addees;
    }
    if (asModerator) {
        stream << " as_moderator=" << *asModerator;
    }
}

}}}
