/**
 * intermediate_format/events/RawContactRequestAcceptEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawContactRequestAcceptEvent.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawContactRequestAcceptEvent::RawContactRequestAcceptEvent(
    IMM(ApparentTime) timestamp,
    unsigned int index,
    TAKE(ApparentSubject) acceptor,
    TAKE(ApparentSubject) acceptee
): RawEvent(timestamp, index), acceptor(move(acceptor)), acceptee(move(acceptee)) {
}

QString RawContactRequestAcceptEvent::eventName() const {
    return "ContactRequestAccept";
}

void RawContactRequestAcceptEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " acceptor=" << acceptor.get() << " acceptee=" << acceptee.get();
}

}}}
