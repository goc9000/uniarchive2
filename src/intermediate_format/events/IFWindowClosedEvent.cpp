/**
 * intermediate_format/events/IFWindowClosedEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFWindowClosedEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFWindowClosedEvent::IFWindowClosedEvent(IMM(ApparentTime) timestamp, unsigned int index)
    : IntermediateFormatEvent(timestamp, index) {
}

QString IFWindowClosedEvent::eventName() const {
    return "WindowClosed";
}

void IFWindowClosedEvent::writeDetailsToDebugStream(QDebug stream) const {
}

}}}
