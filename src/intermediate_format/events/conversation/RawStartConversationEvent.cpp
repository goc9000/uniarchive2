/**
 * intermediate_format/events/conversation/RawStartConversationEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conversation/RawStartConversationEvent.h"

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

RawStartConversationEvent::RawStartConversationEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) initiator,
    TAKE(ApparentSubject) peer
): RawEvent(timestamp, index), initiator(move(initiator)), peer(move(peer)) {
}

QString RawStartConversationEvent::eventName() const {
    return "StartConversation";
}

void RawStartConversationEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (initiator) {
        stream << " initiator=" << initiator.get();
    }
    if (peer) {
        stream << " peer=" << peer.get();
    }
}

}}}
