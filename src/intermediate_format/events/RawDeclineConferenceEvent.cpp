/**
 * intermediate_format/events/RawDeclineConferenceEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/RawDeclineConferenceEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawDeclineConferenceEvent::RawDeclineConferenceEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) subject
): RawEvent(timestamp, index), subject(move(subject)) {
}

QString RawDeclineConferenceEvent::eventName() const {
    return "DeclineConference";
}

void RawDeclineConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
    if (message) {
        stream << " message=" << *message;
    }
}

}}}
