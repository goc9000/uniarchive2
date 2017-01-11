/**
 * intermediate_format/events/conference/RawJoinConferenceEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 9b0f917356fd631251aa15b8054b5447040093a5b09b4dbd779f962b6142b34b
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawJoinConferenceEvent.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawJoinConferenceEvent::RawJoinConferenceEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) subject
) : RawEvent(timestamp, index), subject(move(subject)) {
    sanityCheckMandatoryParameters();
}

void RawJoinConferenceEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
}

RawEventSubType RawJoinConferenceEvent::subType() const {
    return RawEventSubType::JOIN_CONFERENCE;
}

CEDE(RawJoinConferenceEvent) RawJoinConferenceEvent::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::JOIN_CONFERENCE);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    unique_ptr<ApparentSubject> subject = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);
    optional<RawMessageContent> message = must_deserialize(mut_stream, optional<RawMessageContent>);

    unique_ptr<RawJoinConferenceEvent> event = make_unique<RawJoinConferenceEvent>(timestamp, index, move(subject));
    event->skypeGUID = skype_guid;
    event->message = move(message);

    return event;
}

void RawJoinConferenceEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << subject << message;
}

void RawJoinConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject;
    if (message) {
        stream << " message=" << message;
    }
}

}}}
