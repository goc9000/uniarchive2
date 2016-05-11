/**
 * intermediate_format/events/IFStatusChangeEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFStatusChangeEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFStatusChangeEvent::IFStatusChangeEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) subject,
    IMStatus status
): IntermediateFormatEvent(timestamp, index), subject(move(subject)), status(status) {
}

QString IFStatusChangeEvent::eventName() const {
    return "StatusChange";
}

void IFStatusChangeEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get() << " status=" << status;
    if (message) {
        stream << " message=" << *message;
    }
}

}}}
