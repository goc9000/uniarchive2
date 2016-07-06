/**
 * intermediate_format/events/RawContactRequestEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawContactRequestEvent.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawContactRequestEvent::RawContactRequestEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) sender,
    TAKE(ApparentSubject) receiver,
    RawMessageContent&& message
): RawEvent(timestamp, index), sender(move(sender)), receiver(move(receiver)), message(move(message)) {
}

QString RawContactRequestEvent::eventName() const {
    return "ContactRequest";
}

void RawContactRequestEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender.get() << " receiver=" << receiver.get() << " message=" << message;
}

}}}
