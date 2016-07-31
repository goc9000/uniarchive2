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
): RawFailableEvent(timestamp, index), sender(move(sender)), content(move(content)) {
}

QString RawMessageEvent::eventName() const {
    return isAction ? "ActionMessage" : "Message";
}

void RawMessageEvent::writeFailableEventDetailsToDebugStream(QDebug stream) const {
    if (isOffline) {
        stream << " OFFLINE";
    }
    if (isEdited) {
        stream << " EDITED";
        if (editedBy || timeEdited.isSpecified()) {
            stream << "(";
            if (editedBy) {
                stream << "edited_by=" << editedBy.get();
            }
            if (timeEdited.isSpecified()) {
                stream << (editedBy ? " " : "") << "time_edited=" << timeEdited;
            }
            stream << ")";
        }
    }
    stream << " sender=" << sender.get();
    if (receiver) {
        stream << " receiver=" << receiver.get();
    }
    stream << " content=" << content;
}

}}}
