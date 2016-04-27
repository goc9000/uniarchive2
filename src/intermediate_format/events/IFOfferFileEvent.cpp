/**
 * intermediate_format/events/IFOfferFileEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFOfferFileEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFOfferFileEvent::IFOfferFileEvent(
    const ApparentTime& timestamp,
    unsigned int index,
    unique_ptr<ApparentSubject> sender,
    const QString& filename
): IntermediateFormatEvent(timestamp, index), sender(move(sender)), filename(filename) {
}

QString IFOfferFileEvent::eventName() const {
    return "OfferFile";
}

void IFOfferFileEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender.get() << " file=" << filename;
}

}}}
