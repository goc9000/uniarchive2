/**
 * intermediate_format/events/IFMessageEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFMessageEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFMessageEvent::IFMessageEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) sender,
    RawMessageContent&& content
): RawEvent(timestamp, index), sender(move(sender)), content(move(content)), isOffline(false) {
}

QString IFMessageEvent::eventName() const {
    return "Message";
}

void IFMessageEvent::writeDetailsToDebugStream(QDebug stream) const {
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
