/**
 * intermediate_format/events/IFAnswerCallEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFAnswerCallEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFAnswerCallEvent::IFAnswerCallEvent(IMM(ApparentTime) timestamp, unsigned int index, TAKE(ApparentSubject) receiver)
    : IntermediateFormatEvent(timestamp, index), receiver(move(receiver)) {
}

QString IFAnswerCallEvent::eventName() const {
    return "AnswerCall";
}

void IFAnswerCallEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " receiver=" << receiver.get();
    if (caller) {
        stream << " caller=" << caller.get();
    }
}

}}}
