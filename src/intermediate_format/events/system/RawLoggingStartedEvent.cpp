/**
 * intermediate_format/events/system/RawLoggingStartedEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/system/RawLoggingStartedEvent.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawLoggingStartedEvent::RawLoggingStartedEvent(IMM(ApparentTime) timestamp, unsigned int index)
    : RawEvent(timestamp, index) {
}

QString RawLoggingStartedEvent::eventName() const {
    return "LoggingStarted";
}

void RawLoggingStartedEvent::writeDetailsToDebugStream(QDebug UNUSED stream) const {
}

}}}
