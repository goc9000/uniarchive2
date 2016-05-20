/**
 * intermediate_format/events/RawEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/RawEvent.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawEvent::RawEvent(IMM(ApparentTime) timestamp, unsigned int index) : timestamp(timestamp), indexInConversation(index) {
};

void RawEvent::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace() << "#" << indexInConversation << " ";
    stream << "[" << timestamp << "] ";
    stream << QP(eventName());
    writeDetailsToDebugStream(stream);
}

void RawEvent::writeDetailsToDebugStream(QDebug stream) const {
}

QDebug operator<< (QDebug stream, RawEvent const * const event) {
    event->writeToDebugStream(stream);
    return stream;
}

}}}
