/**
 * intermediate_format/events/RawJoinConversationEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawJoinConversationEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawJoinConversationEvent::RawJoinConversationEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) subject
): RawEvent(timestamp, index), subject(move(subject)) {
}

QString RawJoinConversationEvent::eventName() const {
    return "JoinConversation";
}

void RawJoinConversationEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
}

}}}
