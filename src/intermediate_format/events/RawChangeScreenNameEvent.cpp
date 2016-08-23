/**
 * intermediate_format/events/RawChangeScreenNameEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawChangeScreenNameEvent.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawChangeScreenNameEvent::RawChangeScreenNameEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) from,
    TAKE(ApparentSubject) to
) : RawEvent(timestamp, index), from(move(from)), to(move(to)) {
}

QString RawChangeScreenNameEvent::eventName() const {
    return "ChangeScreenName";
}

void RawChangeScreenNameEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " from=" << from.get();
    stream << " to=" << to.get();
}

}}}
