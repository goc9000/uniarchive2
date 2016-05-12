/**
 * intermediate_format/events/IFChangeScreenNameEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFChangeScreenNameEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFChangeScreenNameEvent::IFChangeScreenNameEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) from,
    TAKE(ApparentSubject) to
) : RawEvent(timestamp, index), from(move(from)), to(move(to)) {
}

QString IFChangeScreenNameEvent::eventName() const {
    return "ChangeScreenName";
}

void IFChangeScreenNameEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " from=" << from.get();
    stream << " to=" << to.get();
}

}}}
