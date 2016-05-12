/**
 * intermediate_format/events/IFStartConversationEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFStartConversationEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFStartConversationEvent::IFStartConversationEvent(IMM(ApparentTime) timestamp, unsigned int index)
    : RawEvent(timestamp, index) {
}

IFStartConversationEvent::IFStartConversationEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) initiator
): RawEvent(timestamp, index), initiator(move(initiator)) {
}

QString IFStartConversationEvent::eventName() const {
    return "StartConversation";
}

void IFStartConversationEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (initiator) {
        stream << " initiator=" << initiator.get();
    }
}

}}}
