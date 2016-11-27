/**
 * intermediate_format/events/RawSendContactsEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 20a0e7f093ef57349c89bd1e482b5fb4f559552bde341f4cdd93f60dc988656f
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

RawEventSubType RawSendContactsEvent::subType() const {
    return RawEventSubType::SEND_CONTACTS;
}

void RawSendContactsEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    // TODO: add field serialization code
}

void RawSendContactsEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender.get() << " contacts=" << contacts;
}

}}}
