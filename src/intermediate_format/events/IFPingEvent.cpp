/**
 * intermediate_format/events/IFPingEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFPingEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFPingEvent::IFPingEvent(IMM(ApparentTime) timestamp, unsigned int index, TAKE(ApparentSubject) pinger)
    : RawEvent(timestamp, index), pinger(move(pinger)) {
}

QString IFPingEvent::eventName() const {
    return "Ping";
}

void IFPingEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " pinger=" << pinger.get();
}

}}}
