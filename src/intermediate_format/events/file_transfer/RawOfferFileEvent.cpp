/**
 * intermediate_format/events/file_transfer/RawOfferFileEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/file_transfer/RawOfferFileEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawOfferFileEvent::RawOfferFileEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) sender,
    IMM(QString) filename
): RawEvent(timestamp, index), sender(move(sender)), filename(filename) {
}

QString RawOfferFileEvent::eventName() const {
    return "OfferFile";
}

void RawOfferFileEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender.get() << " file=" << filename;
    if (recipient) {
        stream << " recipient=" << recipient.get();
    }
}

}}}
