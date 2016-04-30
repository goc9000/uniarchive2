/**
 * intermediate_format/events/IFOfferCallEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/IFOfferCallEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

IFOfferCallEvent::IFOfferCallEvent(IMM(ApparentTime) timestamp, unsigned int index, TAKE(ApparentSubject) caller)
    : IntermediateFormatEvent(timestamp, index), caller(move(caller)) {
}

QString IFOfferCallEvent::eventName() const {
    return "OfferCall";
}

void IFOfferCallEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " caller=" << caller.get();
}

}}}
