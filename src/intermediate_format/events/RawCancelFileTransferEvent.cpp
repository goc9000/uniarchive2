/**
 * intermediate_format/events/RawCancelFileTransferEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/RawCancelFileTransferEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawCancelFileTransferEvent::RawCancelFileTransferEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    IMM(QString) filename
): RawEvent(timestamp, index), filename(filename) {
}

QString RawCancelFileTransferEvent::eventName() const {
    return "CancelFileTransfer";
}

void RawCancelFileTransferEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (sender) {
        stream << " sender=" << sender.get();
    }
    stream << " file=" << filename;
}

}}}
