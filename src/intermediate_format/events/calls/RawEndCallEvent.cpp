/**
 * intermediate_format/events/calls/RawEndCallEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/calls/RawEndCallEvent.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawEndCallEvent::RawEndCallEvent(IMM(ApparentTime) timestamp, unsigned int index) : RawEvent(timestamp, index) {
}

QString RawEndCallEvent::eventName() const {
    return "EndCall";
}

void RawEndCallEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (reasonFailed) {
        stream << " FAILED";
        if (*reasonFailed != CurrentCallFailReason::UNDETERMINED) {
            stream << "(" << *reasonFailed << ")";
        }
    }

    if (ender) {
        stream << " ender=" << ender.get();
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
