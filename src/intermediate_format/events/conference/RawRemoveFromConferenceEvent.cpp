/**
 * intermediate_format/events/conference/RawRemoveFromConferenceEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawRemoveFromConferenceEvent.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawRemoveFromConferenceEvent::RawRemoveFromConferenceEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) remover,
    TAKE(ApparentSubject) removee
): RawEvent(timestamp, index), remover(move(remover)), removee(move(removee)) {
}

RawRemoveFromConferenceEvent::RawRemoveFromConferenceEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) removee
): RawEvent(timestamp, index), removee(move(removee)) {
}

QString RawRemoveFromConferenceEvent::eventName() const {
    return "RemoveFromConference";
}

void RawRemoveFromConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (remover) {
        stream << " remover=" << remover.get();
    }
    stream << " removee=" << removee.get();
}

}}}
