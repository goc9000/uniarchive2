/**
 * intermediate_format/events/RawEditedPreviousMessageEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawEditedPreviousMessageEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawEditedPreviousMessageEvent::RawEditedPreviousMessageEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) subject
): RawEvent(timestamp, index), subject(move(subject)) {
}

QString RawEditedPreviousMessageEvent::eventName() const {
    return "EditedPreviousMessage";
}

void RawEditedPreviousMessageEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
}

}}}
