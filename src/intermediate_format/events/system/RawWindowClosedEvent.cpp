/**
 * intermediate_format/events/system/RawWindowClosedEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/system/RawWindowClosedEvent.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawWindowClosedEvent::RawWindowClosedEvent(IMM(ApparentTime) timestamp, unsigned int index)
    : RawEvent(timestamp, index) {
}

QString RawWindowClosedEvent::eventName() const {
    return "WindowClosed";
}

void RawWindowClosedEvent::writeDetailsToDebugStream(QDebug UNUSED stream) const {
}

}}}
