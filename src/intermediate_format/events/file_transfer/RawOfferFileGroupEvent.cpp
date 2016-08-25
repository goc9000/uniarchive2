/**
 * intermediate_format/events/file_transfer/RawOfferFileGroupEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/file_transfer/RawOfferFileGroupEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawOfferFileGroupEvent::RawOfferFileGroupEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) sender,
    uint num_files
) : RawEvent(timestamp, index), sender(move(sender)), numFiles(num_files) {
}

QString RawOfferFileGroupEvent::eventName() const {
    return "OfferFileGroup";
}

void RawOfferFileGroupEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender.get() << " num_files=" << numFiles;
    if (recipient) {
        stream << " recipient=" << recipient.get();
    }
}

}}}
