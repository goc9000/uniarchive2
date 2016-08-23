/**
 * intermediate_format/events/calls/RawStartCallEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/calls/RawStartCallEvent.h"
#include "utils/qt/debug_extras.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawStartCallEvent::RawStartCallEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) initiator,
    TAKE_VEC(ApparentSubject) peers
) : RawFailableEvent(timestamp, index), initiator(move(initiator)), peers(move(peers)) {
}

QString RawStartCallEvent::eventName() const {
    return "StartCall";
}

void RawStartCallEvent::writeFailableEventDetailsToDebugStream(QDebug stream) const {
    stream << " initiator=" << initiator.get();

    if (peers.size() == 1) {
        stream << " peer=" << peers.front();
    } else if (peers.size() > 1) {
        stream << " peers=" << peers;
    }

    if (durationSeconds) {
        stream << " duration=" << *durationSeconds;
    }

    if (skypeCallGUID) {
        stream << " skype_call_guid=" << *skypeCallGUID;
    }
    if (syntheticCallGUID) {
        stream << " synthetic_call_guid=" << *syntheticCallGUID;
    }
    if (correspondingSkypeCallID) {
        stream << " corresp_skype_call_id=" << *correspondingSkypeCallID;
    }
}

}}}
