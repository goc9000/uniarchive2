/**
 * intermediate_format/events/RawUninterpretedEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawUninterpretedEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawUninterpretedEvent::RawUninterpretedEvent(IMM(ApparentTime) timestamp, uint index, IMM(QByteArray) raw_data)
    : RawEvent(timestamp, index), rawData(raw_data) {
}

QString RawUninterpretedEvent::eventName() const {
    return "Uninterpreted";
}

void RawUninterpretedEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " data=" << rawData;
}

}}}
