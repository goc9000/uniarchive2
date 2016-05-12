/**
 * intermediate_format/events/IFCancelFileTransferEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFCancelFileTransferEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFCancelFileTransferEvent::IFCancelFileTransferEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    IMM(QString) filename
): RawEvent(timestamp, index), filename(filename) {
}

QString IFCancelFileTransferEvent::eventName() const {
    return "CancelFileTransfer";
}

void IFCancelFileTransferEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (sender) {
        stream << " sender=" << sender.get();
    }
    stream << " file=" << filename;
}

}}}
