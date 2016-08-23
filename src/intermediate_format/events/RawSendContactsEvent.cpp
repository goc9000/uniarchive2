/**
 * intermediate_format/events/RawSendContactsEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawSendContactsEvent.h"
#include "utils/qt/debug_extras.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawSendContactsEvent::RawSendContactsEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) sender,
    TAKE_VEC(ApparentSubject) contacts
): RawEvent(timestamp, index), sender(move(sender)), contacts(move(contacts)) {
}

QString RawSendContactsEvent::eventName() const {
    return "SendContacts";
}

void RawSendContactsEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender.get() << " contacts=" << contacts;
}

}}}
