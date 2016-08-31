/**
 * intermediate_format/events/RawSendContactsEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 781bd9a5048beb43b945406c9614cb6ff38a4066523d6746e770be3a8198ffd9
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawSendContactsEvent.h"
#include "utils/qt/debug_extras.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawSendContactsEvent::RawSendContactsEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) sender,
    TAKE_VEC(ApparentSubject) contacts
) : RawEvent(timestamp, index), sender(move(sender)), contacts(move(contacts)) {
    sanityCheckMandatoryParameters();
}

void RawSendContactsEvent::sanityCheckMandatoryParameters() const {
    invariant(sender, "Parameter 'sender' cannot have empty value");
    for (IMM(auto) item : contacts) {
        invariant(item, "Parameter 'contacts' cannot have empty entries");
    }
}

QString RawSendContactsEvent::eventName() const {
    return "SendContacts";
}

void RawSendContactsEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender.get() << " contacts=" << contacts;
}

}}}
