/**
 * intermediate_format/events/RawOfferFileGroupEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/RawOfferFileGroupEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawOfferFileGroupEvent::RawOfferFileGroupEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) sender,
    unsigned int num_files
): RawEvent(timestamp, index), sender(move(sender)), num_files(num_files) {
}

QString RawOfferFileGroupEvent::eventName() const {
    return "OfferFileGroup";
}

void RawOfferFileGroupEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender.get() << " num_files=" << num_files;
    if (recipient) {
        stream << " recipient=" << recipient.get();
    }
}

}}}
