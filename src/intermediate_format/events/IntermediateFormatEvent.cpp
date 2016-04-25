/**
 * intermediate_format/events/IntermediateFormatEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IntermediateFormatEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IntermediateFormatEvent::IntermediateFormatEvent(const ApparentTime& timestamp, unsigned int index)
    : timestamp(timestamp), indexInConversation(index) {
};

void IntermediateFormatEvent::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace() << "#" << indexInConversation << " ";
    stream << "[" << timestamp << "] ";
    stream << qPrintable(eventName());
    writeDetailsToDebugStream(stream);
}

void IntermediateFormatEvent::writeDetailsToDebugStream(QDebug stream) const {
}

QDebug operator<< (QDebug stream, IntermediateFormatEvent const * const event) {
    event->writeToDebugStream(stream);
    return stream;
}

}}}
