/**
 * intermediate_format/events/conversation/RawLeaveConversationEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 3bd3ae97e339e6e0376b0d388ab98120c2bb61e7a2e131bc33a7201c82ac2861
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conversation/RawLeaveConversationEvent.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawLeaveConversationEvent::RawLeaveConversationEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) subject
) : RawEvent(timestamp, index), subject(move(subject)) {
    sanityCheckMandatoryParameters();
}

void RawLeaveConversationEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
}

RawEventSubType RawLeaveConversationEvent::subType() const {
    return RawEventSubType::LEAVE_CONVERSATION;
}

bool RawLeaveConversationEvent::visitSubjectsImpl(IApparentSubjectVisitor& UNUSED visitor) {
    return true;
}

CEDE(RawLeaveConversationEvent) RawLeaveConversationEvent::deserializeFromStream(
    QDataStream& mut_stream,
    bool skip_type
) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::LEAVE_CONVERSATION);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    unique_ptr<ApparentSubject> subject = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);

    unique_ptr<RawLeaveConversationEvent> event = make_unique<RawLeaveConversationEvent>(
        timestamp,
        index,
        move(subject)
    );
    event->skypeGUID = skype_guid;

    return event;
}

void RawLeaveConversationEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << subject;
}

void RawLeaveConversationEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject;
}

}}}
