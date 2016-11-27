/**
 * intermediate_format/events/conversation/RawJoinConversationEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 51f7c4f941af1d67ccaf9f2f9c2f6b53d0481378913f7ccc0ca5126b5d25af10
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conversation/RawJoinConversationEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawJoinConversationEvent::RawJoinConversationEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) subject
) : RawEvent(timestamp, index), subject(move(subject)) {
    sanityCheckMandatoryParameters();
}

void RawJoinConversationEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
}

RawEventSubType RawJoinConversationEvent::subType() const {
    return RawEventSubType::JOIN_CONVERSATION;
}

void RawJoinConversationEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    // TODO: add field serialization code
}

void RawJoinConversationEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
}

}}}
