/**
 * intermediate_format/events/file_transfer/RawCancelFileTransferEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/file_transfer/RawCancelFileTransferEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawCancelFileTransferEvent::RawCancelFileTransferEvent(IMM(ApparentTime) timestamp, uint index)
    : RawEvent(timestamp, index) {
}

RawCancelFileTransferEvent::RawCancelFileTransferEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    IMM(RawTransferredFile) file
): RawEvent(timestamp, index), file(file) {
}

QString RawCancelFileTransferEvent::eventName() const {
    return "CancelFileTransfer";
}

void RawCancelFileTransferEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (actor) {
        stream << " actor=" << actor.get();
    }
    if (file) {
        stream << " file=" << *file;
    }
}

}}}
