/**
 * intermediate_format/events/RawCorruptedMessageEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawCorruptedMessageEvent.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawCorruptedMessageEvent::RawCorruptedMessageEvent(IMM(ApparentTime) timestamp, uint index, IMM(QByteArray) raw_data)
    : RawEvent(timestamp, index), rawData(raw_data) {
}

QString RawCorruptedMessageEvent::eventName() const {
    return "CorruptedMessage";
}

void RawCorruptedMessageEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " data=" << rawData;
}

}}}
