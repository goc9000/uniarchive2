/**
 * intermediate_format/events/conference/RawChangeTopicEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): eb946a3c2fd4f6618ac40e8a6486a647dc8bea42597e7f379238e6037b908a33
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawChangeTopicEvent.h"
#include "utils/qt/debug_extras.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawChangeTopicEvent::RawChangeTopicEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
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

void RawChangeTopicEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    // TODO: add field serialization code
}

void RawChangeTopicEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject << " topic=" << topic;
}

}}}
