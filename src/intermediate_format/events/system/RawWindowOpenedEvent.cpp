/**
 * intermediate_format/events/system/RawWindowOpenedEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/system/RawWindowOpenedEvent.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawWindowOpenedEvent::RawWindowOpenedEvent(IMM(ApparentTime) timestamp, uint index) : RawEvent(timestamp, index) {
}

QString RawWindowOpenedEvent::eventName() const {
    return "WindowOpened";
}

void RawWindowOpenedEvent::writeDetailsToDebugStream(QDebug UNUSED stream) const {
}

}}}
