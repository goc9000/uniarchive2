/**
 * intermediate_format/events/IFDeclineConferenceEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFDeclineConferenceEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFDeclineConferenceEvent::IFDeclineConferenceEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) subject
): IntermediateFormatEvent(timestamp, index), subject(move(subject)) {
}

QString IFDeclineConferenceEvent::eventName() const {
    return "DeclineConference";
}

void IFDeclineConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
    if (message) {
        stream << " message=" << *message;
    }
}

}}}
