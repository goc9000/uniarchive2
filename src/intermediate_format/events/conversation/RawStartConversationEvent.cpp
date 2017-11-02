/**
 * intermediate_format/events/conversation/RawStartConversationEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): aea92415215dd830a5a405893e5f423fa8087c3f3175f4ca7cead9e4b0d6aa5f
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conversation/RawStartConversationEvent.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawStartConversationEvent::RawStartConversationEvent(IMM(ApparentTime) timestamp, uint32_t index)
  : RawEvent(timestamp, index) {
    // Nothing else to initialize
}

RawStartConversationEvent::RawStartConversationEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) initiator
) : RawEvent(timestamp, index), initiator(move(initiator)) {
    invariant(this->initiator, "Parameter 'initiator' cannot have empty value");
}

RawStartConversationEvent::RawStartConversationEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) initiator,
    TAKE(ApparentSubject) peer
) : RawEvent(timestamp, index), initiator(move(initiator)), peer(move(peer)) {
    invariant(this->initiator, "Parameter 'initiator' cannot have empty value");
    invariant(this->peer, "Parameter 'peer' cannot have empty value");
}

RawEventSubType RawStartConversationEvent::subType() const {
    return RawEventSubType::START_CONVERSATION;
}

bool RawStartConversationEvent::visitSubjectsImpl(IApparentSubjectVisitor& UNUSED visitor) {
    return true;
}

CEDE(RawStartConversationEvent) RawStartConversationEvent::deserializeFromStream(
    QDataStream& mut_stream,
    bool skip_type
) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::START_CONVERSATION);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    unique_ptr<ApparentSubject> initiator = must_deserialize_optional_unique_ptr(mut_stream, ApparentSubject);
    unique_ptr<ApparentSubject> peer = must_deserialize_optional_unique_ptr(mut_stream, ApparentSubject);

    unique_ptr<RawStartConversationEvent> event = make_unique<RawStartConversationEvent>(timestamp, index);
    event->skypeGUID = skype_guid;
    event->initiator = move(initiator);
    event->peer = move(peer);

    return event;
}

void RawStartConversationEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    serialize_optional_unique_ptr(mut_stream, initiator);
    serialize_optional_unique_ptr(mut_stream, peer);
}

void RawStartConversationEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (initiator) {
        stream << " initiator=" << initiator;
    }
    if (peer) {
        stream << " peer=" << peer;
    }
}

}}}
