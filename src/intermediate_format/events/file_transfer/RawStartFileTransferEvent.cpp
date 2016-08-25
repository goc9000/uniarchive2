/**
 * intermediate_format/events/file_transfer/RawStartFileTransferEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/file_transfer/RawStartFileTransferEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawStartFileTransferEvent::RawStartFileTransferEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    IMM(RawTransferredFile) file
): RawEvent(timestamp, index), file(file) {
}

QString RawStartFileTransferEvent::eventName() const {
    return "StartFileTransfer";
}

void RawStartFileTransferEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (sender) {
        stream << " sender=" << sender.get();
    }
    stream << " file=" << file;
}

}}}
