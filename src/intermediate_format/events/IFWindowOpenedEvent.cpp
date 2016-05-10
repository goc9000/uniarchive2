/**
 * intermediate_format/events/IFWindowOpenedEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFWindowOpenedEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFWindowOpenedEvent::IFWindowOpenedEvent(IMM(ApparentTime) timestamp, unsigned int index)
    : IntermediateFormatEvent(timestamp, index) {
}

QString IFWindowOpenedEvent::eventName() const {
    return "WindowOpened";
}

void IFWindowOpenedEvent::writeDetailsToDebugStream(QDebug stream) const {
}

}}}
