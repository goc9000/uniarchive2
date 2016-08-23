/**
 * intermediate_format/events/file_transfer/RawReceiveFileEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/file_transfer/RawReceiveFileEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawReceiveFileEvent::RawReceiveFileEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) receiver,
    IMM(RawTransferredFile) file
): RawEvent(timestamp, index), receiver(move(receiver)), file(file) {
}

QString RawReceiveFileEvent::eventName() const {
    return "ReceiveFile";
}

void RawReceiveFileEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " receiver=" << receiver.get();
    if (sender) {
        stream << " sender=" << sender.get();
    }
    stream << " file=" << file;
}

}}}
