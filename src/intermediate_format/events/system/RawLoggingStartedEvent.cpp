/**
 * intermediate_format/events/system/RawLoggingStartedEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 7b6f1281823fc48e5cdfe0bc5d272f479f6841d9b311c71c945b94ee1d4cd30a
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/system/RawLoggingStartedEvent.h"
#include "utils/serialization/deserialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawLoggingStartedEvent::RawLoggingStartedEvent(IMM(ApparentTime) timestamp, uint32_t index)
  : RawEvent(timestamp, index) {
    // Nothing else to initialize
}

RawEventSubType RawLoggingStartedEvent::subType() const {
    return RawEventSubType::LOGGING_STARTED;
}

bool RawLoggingStartedEvent::visitSubjectsImpl(IApparentSubjectVisitor& UNUSED visitor) {
    return true;
}

CEDE(RawLoggingStartedEvent) RawLoggingStartedEvent::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::LOGGING_STARTED);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);

    unique_ptr<RawLoggingStartedEvent> event = make_unique<RawLoggingStartedEvent>(timestamp, index);
    event->skypeGUID = skype_guid;

    return event;
}

void RawLoggingStartedEvent::serializeDetailsToStream(QDataStream& UNUSED mut_stream) const {
    // No fields to serialize
}

void RawLoggingStartedEvent::writeDetailsToDebugStream(QDebug UNUSED stream) const {
    // No fields to write
}

}}}
