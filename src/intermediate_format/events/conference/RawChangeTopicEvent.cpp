/**
 * intermediate_format/events/conference/RawChangeTopicEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 050cd9333269bcd00f8ff880bb72a0602edcf89e3cc8d3ea9c0b7044cd0b0d38
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
) : RawEvent(timestamp, index), subject(move(subject)), topic(move(topic)) {
}

QString RawChangeTopicEvent::eventName() const {
    return "ChangeTopic";
}

void RawChangeTopicEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get() << " topic=" << topic;
}

}}}
