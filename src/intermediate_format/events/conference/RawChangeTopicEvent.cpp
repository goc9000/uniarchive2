/**
 * intermediate_format/events/conference/RawChangeTopicEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 916b0e6dda04ba18a758e7789ee873cdda0bdf7bb761966c5b172ca0966f1326
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
    sanityCheckMandatoryParameters();
}

void RawChangeTopicEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
}

RawEventSubType RawChangeTopicEvent::subType() const {
    return RawEventSubType::CHANGE_TOPIC;
}

QString RawChangeTopicEvent::eventName() const {
    return "ChangeTopic";
}

void RawChangeTopicEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get() << " topic=" << topic;
}

}}}
