/**
 * intermediate_format/events/RawMessageSendFailedEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawMessageSendFailedEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawMessageSendFailedEvent::RawMessageSendFailedEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) sender,
    SendFailReason reason,
    RawMessageContent&& unsentMessageContent
): RawEvent(timestamp, index), sender(move(sender)), reasonFailed(reason),
   unsentMessageContent(move(unsentMessageContent)) {
}

QString RawMessageSendFailedEvent::eventName() const {
    return "MessageSendFailed";
}

void RawMessageSendFailedEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender.get();
    if (receiver) {
        stream << " receiver=" << receiver.get();
    }
    stream << " reason=";
    switch (reasonFailed) {
        case SendFailReason::UNKNOWN:
            stream << "UNKNOWN";
            break;
        case SendFailReason::MESSAGE_TOO_LARGE:
            stream << "MESSAGE_TOO_LARGE";
            break;
        case SendFailReason::RECIPIENT_OFFLINE:
            stream << "RECIPIENT_OFFLINE";
            break;
        case SendFailReason::CONNECTION_ERROR:
            stream << "CONNECTION_ERROR";
            break;
    }
    if (!unsentMessageContent.items.empty()) {
        stream << " unsent_message=" << unsentMessageContent;
    }
}

}}}
