/**
 * intermediate_format/events/RawLeaveConversationEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/RawLeaveConversationEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawLeaveConversationEvent::RawLeaveConversationEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) subject
): RawEvent(timestamp, index), subject(move(subject)) {
}

QString RawLeaveConversationEvent::eventName() const {
    return "LeaveConversation";
}

void RawLeaveConversationEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
}

}}}
