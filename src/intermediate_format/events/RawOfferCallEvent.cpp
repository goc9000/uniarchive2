/**
 * intermediate_format/events/RawOfferCallEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/RawOfferCallEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawOfferCallEvent::RawOfferCallEvent(IMM(ApparentTime) timestamp, unsigned int index, TAKE(ApparentSubject) caller)
    : RawEvent(timestamp, index), caller(move(caller)) {
}

QString RawOfferCallEvent::eventName() const {
    return "OfferCall";
}

void RawOfferCallEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " caller=" << caller.get();
}

}}}
