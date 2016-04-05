/**
 * intermediate_format/events/IFJoinConferenceEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFJoinConferenceEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFJoinConferenceEvent::IFJoinConferenceEvent(
    ApparentTime timestamp,
    unsigned int index,
    shared_ptr<ApparentSubject> subject
): IntermediateFormatEvent(timestamp, index), subject(subject) {
}

QString IFJoinConferenceEvent::eventName() const {
    return "JoinConference";
}

void IFJoinConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
    if (message) {
        stream << " message=" << *message;
    }
}

}}}
