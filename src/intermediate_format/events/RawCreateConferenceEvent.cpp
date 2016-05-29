/**
 * intermediate_format/events/RawCreateConferenceEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/RawCreateConferenceEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawCreateConferenceEvent::RawCreateConferenceEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) creator,
    IMM(QString) conferenceName
): RawEvent(timestamp, index), creator(move(creator)), conferenceName(conferenceName) {
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

