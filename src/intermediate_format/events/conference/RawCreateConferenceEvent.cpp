/**
 * intermediate_format/events/conference/RawCreateConferenceEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawCreateConferenceEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawCreateConferenceEvent::RawCreateConferenceEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) creator,
    IMM(QString) conference_name
) : RawEvent(timestamp, index), creator(move(creator)), conferenceName(conference_name) {
}

QString RawCreateConferenceEvent::eventName() const {
    return "CreateConference";
}

void RawCreateConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " creator=" << creator.get();
    if (!conferenceName.isEmpty()) {
        stream << " conference_name=" << conferenceName;
    }
}

}}}

