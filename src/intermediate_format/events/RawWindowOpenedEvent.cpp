/**
 * intermediate_format/events/RawWindowOpenedEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/RawWindowOpenedEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawWindowOpenedEvent::RawWindowOpenedEvent(IMM(ApparentTime) timestamp, unsigned int index)
    : RawEvent(timestamp, index) {
}

QString RawWindowOpenedEvent::eventName() const {
    return "WindowOpened";
}

void RawWindowOpenedEvent::writeDetailsToDebugStream(QDebug stream) const {
}

}}}
