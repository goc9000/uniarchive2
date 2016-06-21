/**
 * intermediate_format/events/RawStartConversationEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawStartConversationEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawStartConversationEvent::RawStartConversationEvent(IMM(ApparentTime) timestamp, unsigned int index)
    : RawEvent(timestamp, index) {
}

RawStartConversationEvent::RawStartConversationEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) initiator
): RawEvent(timestamp, index), initiator(move(initiator)) {
}

QString RawStartConversationEvent::eventName() const {
    return "StartConversation";
}

void RawStartConversationEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (initiator) {
        stream << " initiator=" << initiator.get();
    }
}

}}}
