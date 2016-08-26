/**
 * intermediate_format/events/conversation/RawLeaveConversationEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): cf1ab588b8d7cfa0e359dad1adf15962704f9449fafe070a93041aa21a3d52d4
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conversation/RawLeaveConversationEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawLeaveConversationEvent::RawLeaveConversationEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) subject
) : RawEvent(timestamp, index), subject(move(subject)) {
}

QString RawLeaveConversationEvent::eventName() const {
    return "LeaveConversation";
}

void RawLeaveConversationEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
}

}}}
