/**
 * intermediate_format/events/RawEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawEvent.h"
#include "utils/language/shortcuts.h"
#include "utils/qt/shortcuts.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawEvent::RawEvent(IMM(ApparentTime) timestamp, uint index) : timestamp(timestamp), indexInConversation(index) {
}

void RawEvent::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace() << "#" << indexInConversation << " ";
    stream << "[" << timestamp << "] ";
    stream << QP(eventName());
    writeDetailsToDebugStream(stream);

    if (skypeGUID) {
        stream << " skype_guid=" << *skypeGUID;
    }
}

void RawEvent::writeDetailsToDebugStream(QDebug UNUSED stream) const {
}

QDebug operator<< (QDebug stream, CPTR(RawEvent) event) {
    event->writeToDebugStream(stream);
    return stream;
}

}}}
