/**
 * intermediate_format/events/calls/RawEndCallEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 35d42085ad7114628bf5b29288e3f4c865979693d56536804856fcbd7cd42d2d
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/calls/RawEndCallEvent.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawEndCallEvent::RawEndCallEvent(IMM(ApparentTime) timestamp, uint32_t index) : RawEvent(timestamp, index) {
    // Nothing else to initialize
}

RawEventSubType RawEndCallEvent::subType() const {
    return RawEventSubType::END_CALL;
}

bool RawEndCallEvent::visitSubjectsImpl(IApparentSubjectVisitor& UNUSED visitor) {
    return true;
}

CEDE(RawEndCallEvent) RawEndCallEvent::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::END_CALL);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    unique_ptr<ApparentSubject> ender = must_deserialize_optional_unique_ptr(mut_stream, ApparentSubject);
    optional<CurrentCallFailReason> reason_call_failed = must_deserialize(mut_stream, optional<CurrentCallFailReason>);
    optional<QString> skype_call_guid = must_deserialize(mut_stream, optional<QString>);
    optional<QString> synthetic_call_guid = must_deserialize(mut_stream, optional<QString>);
    optional<uint64_t> corresponding_skype_call_id = must_deserialize(mut_stream, optional<uint64_t>);

    unique_ptr<RawEndCallEvent> event = make_unique<RawEndCallEvent>(timestamp, index);
    event->skypeGUID = skype_guid;
    event->ender = move(ender);
    event->reasonCallFailed = reason_call_failed;
    event->skypeCallGUID = skype_call_guid;
    event->syntheticCallGUID = synthetic_call_guid;
    event->correspondingSkypeCallID = corresponding_skype_call_id;

    return event;
}

void RawEndCallEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    serialize_optional_unique_ptr(mut_stream, ender);
    mut_stream << reasonCallFailed << skypeCallGUID << syntheticCallGUID << correspondingSkypeCallID;
}

void RawEndCallEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (ender) {
        stream << " ender=" << ender;
    }
    if (reasonCallFailed) {
        stream << " reason_call_failed=" << reasonCallFailed;
    }
    if (skypeCallGUID) {
        stream << " skype_call_guid=" << skypeCallGUID;
    }
    if (syntheticCallGUID) {
        stream << " synthetic_call_guid=" << syntheticCallGUID;
    }
    if (correspondingSkypeCallID) {
        stream << " corresponding_skype_call_id=" << correspondingSkypeCallID;
    }
}

}}}
