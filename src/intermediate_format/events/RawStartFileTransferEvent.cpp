/**
 * intermediate_format/events/RawStartFileTransferEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/RawStartFileTransferEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawStartFileTransferEvent::RawStartFileTransferEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    IMM(QString) filename
): RawEvent(timestamp, index), filename(filename) {
}

QString RawStartFileTransferEvent::eventName() const {
    return "StartFileTransfer";
}

void RawStartFileTransferEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (sender) {
        stream << " sender=" << sender.get();
    }
    stream << " file=" << filename;
}

}}}
