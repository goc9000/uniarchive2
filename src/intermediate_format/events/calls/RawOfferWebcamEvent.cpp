/**
 * intermediate_format/events/calls/RawOfferWebcamEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/calls/RawOfferWebcamEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawOfferWebcamEvent::RawOfferWebcamEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) sender)
    : RawFailableEvent(timestamp, index), sender(move(sender)) {
}

QString RawOfferWebcamEvent::eventName() const {
    return "OfferWebcam";
}

void RawOfferWebcamEvent::writeFailableEventDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender.get();
}

}}}
