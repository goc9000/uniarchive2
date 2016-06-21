/**
 * intermediate_format/events/RawMessageEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawMessageEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawMessageEvent::RawMessageEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) sender,
    RawMessageContent&& content
): RawEvent(timestamp, index), sender(move(sender)), content(move(content)), isOffline(false) {
}

QString RawMessageEvent::eventName() const {
    return "Message";
}

void RawMessageEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (isOffline) {
        stream << " OFFLINE";
    }
    stream << " sender=" << sender.get();
    if (receiver) {
        stream << " receiver=" << receiver.get();
    }
    stream << " content=" << content;
}

}}}
