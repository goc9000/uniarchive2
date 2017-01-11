/**
 * intermediate_format/events/friending/RawContactRequestEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 2d3c728397c603737f5739e87689062bfc592ae958013fe76b6f851428315c71
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/friending/RawContactRequestEvent.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawContactRequestEvent::RawContactRequestEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) sender,
    TAKE(ApparentSubject) receiver,
    RawMessageContent&& message
) : RawEvent(timestamp, index), sender(move(sender)), receiver(move(receiver)), message(move(message)) {
    sanityCheckMandatoryParameters();
}

void RawContactRequestEvent::sanityCheckMandatoryParameters() const {
    invariant(sender, "Parameter 'sender' cannot have empty value");
    invariant(receiver, "Parameter 'receiver' cannot have empty value");
}

RawEventSubType RawContactRequestEvent::subType() const {
    return RawEventSubType::CONTACT_REQUEST;
}

CEDE(RawContactRequestEvent) RawContactRequestEvent::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::CONTACT_REQUEST);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    unique_ptr<ApparentSubject> sender = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);
    unique_ptr<ApparentSubject> receiver = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);
    RawMessageContent message = must_deserialize(mut_stream, RawMessageContent);

    unique_ptr<RawContactRequestEvent> event = make_unique<RawContactRequestEvent>(
        timestamp,
        index,
        move(sender),
        move(receiver),
        move(message)
    );
    event->skypeGUID = skype_guid;

    return event;
}

void RawContactRequestEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << sender << receiver << message;
}

void RawContactRequestEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender << " receiver=" << receiver << " message=" << message;
}

}}}
