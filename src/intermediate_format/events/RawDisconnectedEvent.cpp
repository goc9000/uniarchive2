/**
 * intermediate_format/events/RawDisconnectedEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/RawDisconnectedEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawDisconnectedEvent::RawDisconnectedEvent(IMM(ApparentTime) timestamp, unsigned int index) : RawEvent(timestamp, index) {
}

QString RawDisconnectedEvent::eventName() const {
    return "Disconnected";
}

void RawDisconnectedEvent::writeDetailsToDebugStream(QDebug stream) const {
}

}}}
