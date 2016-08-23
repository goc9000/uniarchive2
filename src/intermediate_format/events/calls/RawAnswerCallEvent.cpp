/**
 * intermediate_format/events/calls/RawAnswerCallEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/calls/RawAnswerCallEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawAnswerCallEvent::RawAnswerCallEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) receiver)
    : RawEvent(timestamp, index), receiver(move(receiver)) {
}

QString RawAnswerCallEvent::eventName() const {
    return "AnswerCall";
}

void RawAnswerCallEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " receiver=" << receiver.get();
    if (caller) {
        stream << " caller=" << caller.get();
    }
}

}}}
