/**
 * intermediate_format/events/IFStartFileTransferEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFStartFileTransferEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFStartFileTransferEvent::IFStartFileTransferEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    IMM(QString) filename
): IntermediateFormatEvent(timestamp, index), filename(filename) {
}

QString IFStartFileTransferEvent::eventName() const {
    return "StartFileTransfer";
}

void IFStartFileTransferEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (sender) {
        stream << " sender=" << sender.get();
    }
    stream << " file=" << filename;
}

}}}
