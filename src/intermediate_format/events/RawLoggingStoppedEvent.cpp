/**
 * intermediate_format/events/RawLoggingStoppedEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/RawLoggingStoppedEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawLoggingStoppedEvent::RawLoggingStoppedEvent(IMM(ApparentTime) timestamp, unsigned int index)
    : RawEvent(timestamp, index) {
}

QString RawLoggingStoppedEvent::eventName() const {
    return "LoggingStopped";
}

void RawLoggingStoppedEvent::writeDetailsToDebugStream(QDebug stream) const {
}

}}}