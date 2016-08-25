/**
 * intermediate_format/events/conference/RawChangeTopicEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawChangeTopicEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawChangeTopicEvent::RawChangeTopicEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) subject,
    RawMessageContent&& topic
): RawEvent(timestamp, index), subject(move(subject)), topic(move(topic)) {
}

QString RawChangeTopicEvent::eventName() const {
    return "ChangeTopic";
}

void RawChangeTopicEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get() << " topic=" << topic;
}

}}}
