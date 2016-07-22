/**
 * intermediate_format/events/file_transfer/RawTransferFilesEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/file_transfer/RawTransferFilesEvent.h"
#include "utils/qt/debug_extras.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawTransferFilesEvent::RawTransferFilesEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) sender,
    IMM(vector<RawTransferredFile>) files
): RawEvent(timestamp, index), sender(move(sender)), files(files) {
}

QString RawTransferFilesEvent::eventName() const {
    return "TransferFiles";
}

void RawTransferFilesEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender.get();
    if (files.size() != 1) {
        stream << " files=" << files;
    } else {
        stream << " file=" << files.front();
    }
    if (recipient) {
        stream << " recipient=" << recipient.get();
    }
}

}}}
