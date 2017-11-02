/**
 * intermediate_format/events/RawUninterpretedEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 8afb4be9425a727ad766b99d172ff887513902d35403f7e3fca476cf8ebb0346
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawUninterpretedEvent.h"
#include "utils/serialization/deserialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawUninterpretedEvent::RawUninterpretedEvent(IMM(ApparentTime) timestamp, uint32_t index, IMM(QByteArray) raw_data)
  : RawEvent(timestamp, index), rawData(raw_data) {
    // Nothing else to initialize
}

RawEventSubType RawUninterpretedEvent::subType() const {
    return RawEventSubType::UNINTERPRETED;
}

void RawUninterpretedEvent::visitSubjectsImpl(ApparentSubjectVisitor& UNUSED visitor) {
    // No fields to visit
}

CEDE(RawUninterpretedEvent) RawUninterpretedEvent::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::UNINTERPRETED);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    QByteArray raw_data = must_deserialize(mut_stream, QByteArray);

    unique_ptr<RawUninterpretedEvent> event = make_unique<RawUninterpretedEvent>(timestamp, index, raw_data);
    event->skypeGUID = skype_guid;

    return event;
}

void RawUninterpretedEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << rawData;
}

void RawUninterpretedEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " raw_data=" << rawData;
}

}}}
