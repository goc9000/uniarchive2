/**
 * intermediate_format/events/IFDisconnectedEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFDisconnectedEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFDisconnectedEvent::IFDisconnectedEvent(IMM(ApparentTime) timestamp, unsigned int index) : RawEvent(timestamp, index) {
}

QString IFDisconnectedEvent::eventName() const {
    return "Disconnected";
}

void IFDisconnectedEvent::writeDetailsToDebugStream(QDebug stream) const {
}

}}}
