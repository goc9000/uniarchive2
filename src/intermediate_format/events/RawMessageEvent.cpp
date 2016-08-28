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
    uint index,
    TAKE(ApparentSubject) sender,
    RawMessageContent&& content
): RawFailableEvent(timestamp, index), sender(move(sender)), content(move(content)) {
}

QString RawMessageEvent::eventName() const {
    // BEGIN CUSTOM SECTION: Name method
    return isAction ? "ActionMessage" : "Message";
    // END CUSTOM SECTION: Name method
}

void RawMessageEvent::writeFailableEventDetailsToDebugStream(QDebug stream) const {
    // BEGIN CUSTOM SECTION: Debug write method
    if (isOffline) {
        stream << " OFFLINE";
    }
    if (isEdited) {
        stream << " EDITED";
        if (editedBy || timeEdited) {
            stream << "(";
            if (editedBy) {
                stream << "edited_by=" << editedBy.get();
            }
            if (timeEdited) {
                stream << (editedBy ? " " : "") << "time_edited=" << *timeEdited;
            }
            stream << ")";
        }
    }
    stream << " sender=" << sender.get();
    if (receiver) {
        stream << " receiver=" << receiver.get();
    }
    stream << " content=" << content;
    // END CUSTOM SECTION: Debug write method
}

}}}
