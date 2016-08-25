/**
 * intermediate_format/events/file_transfer/RawOfferFileEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/file_transfer/RawOfferFileEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawOfferFileEvent::RawOfferFileEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) sender,
    IMM(RawTransferredFile) file
): RawEvent(timestamp, index), sender(move(sender)), file(file) {
}

QString RawOfferFileEvent::eventName() const {
    return "OfferFile";
}

void RawOfferFileEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender.get() << " file=" << file;
    if (recipient) {
        stream << " recipient=" << recipient.get();
    }
}

}}}
