/**
 * intermediate_format/events/RawCorruptedMessageEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 6418e887a3872ccb400b1d3032f06368d97305c3d9839634dcf719d13b37f457
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawCorruptedMessageEvent.h"
#include "utils/serialization/deserialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawCorruptedMessageEvent::RawCorruptedMessageEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    IMM(QByteArray) raw_data
) : RawEvent(timestamp, index), rawData(raw_data) {
    // Nothing else to initialize
}

RawEventSubType RawCorruptedMessageEvent::subType() const {
    return RawEventSubType::CORRUPTED_MESSAGE;
}

bool RawCorruptedMessageEvent::visitSubjectsImpl(IApparentSubjectVisitor& UNUSED visitor) {
    return true;
}

CEDE(RawCorruptedMessageEvent) RawCorruptedMessageEvent::deserializeFromStream(
    QDataStream& mut_stream,
    bool skip_type
) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::CORRUPTED_MESSAGE);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    QByteArray raw_data = must_deserialize(mut_stream, QByteArray);

    unique_ptr<RawCorruptedMessageEvent> event = make_unique<RawCorruptedMessageEvent>(timestamp, index, raw_data);
    event->skypeGUID = skype_guid;

    return event;
}

void RawCorruptedMessageEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << rawData;
}

void RawCorruptedMessageEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " raw_data=" << rawData;
}

}}}
