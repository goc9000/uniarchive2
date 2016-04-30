/**
 * intermediate_format/events/IFReceiveFileEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFReceiveFileEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFReceiveFileEvent::IFReceiveFileEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    unique_ptr<ApparentSubject> receiver,
    IMM(QString) filename
): IntermediateFormatEvent(timestamp, index), receiver(move(receiver)), filename(filename) {
}

QString IFReceiveFileEvent::eventName() const {
    return "ReceiveFile";
}

void IFReceiveFileEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " receiver=" << receiver.get();
    if (sender) {
        stream << " sender=" << sender.get();
    }
    stream << " file=" << filename;
}

}}}
