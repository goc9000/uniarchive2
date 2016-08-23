/**
 * intermediate_format/events/conference/RawRejoinConferenceEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawRejoinConferenceEvent.h"

#include "utils/qt/debug_extras.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawRejoinConferenceEvent::RawRejoinConferenceEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) subject,
    TAKE_VEC(ApparentSubject) peers
): RawEvent(timestamp, index), subject(move(subject)), peers(move(peers)) {
}

RawRejoinConferenceEvent::RawRejoinConferenceEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) subject
): RawEvent(timestamp, index) {
    peers.push_back(move(subject));
}

QString RawRejoinConferenceEvent::eventName() const {
    return "RejoinConference";
}

void RawRejoinConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
    if (!peers.empty()) {
        stream << " peers=" << peers;
    }
}

}}}
