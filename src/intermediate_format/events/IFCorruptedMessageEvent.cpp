/**
 * intermediate_format/events/IFCorruptedMessageEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFCorruptedMessageEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFCorruptedMessageEvent::IFCorruptedMessageEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    IMM(QByteArray) raw_data
) : RawEvent(timestamp, index), rawData(raw_data) {
}

QString IFCorruptedMessageEvent::eventName() const {
    return "CorruptedMessage";
}

void IFCorruptedMessageEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " data=" << rawData;
}

}}}
