/**
 * intermediate_format/events/IFConnectedEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFConnectedEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFConnectedEvent::IFConnectedEvent(IMM(ApparentTime) timestamp, unsigned int index) : RawEvent(timestamp, index) {
}

QString IFConnectedEvent::eventName() const {
    return "Connected";
}

void IFConnectedEvent::writeDetailsToDebugStream(QDebug stream) const {
}

}}}
