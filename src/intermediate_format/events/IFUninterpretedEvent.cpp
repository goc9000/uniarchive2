/**
 * intermediate_format/events/IFUninterpretedEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFUninterpretedEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFUninterpretedEvent::IFUninterpretedEvent(IMM(ApparentTime) timestamp, unsigned int index, IMM(QByteArray) raw_data)
    : RawEvent(timestamp, index), rawData(raw_data) {
}

QString IFUninterpretedEvent::eventName() const {
    return "Uninterpreted";
}

void IFUninterpretedEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " data=" << rawData;
}

}}}
