/**
 * intermediate_format/events/friending/RawContactRequestAcceptEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 90f1210f995009a5aaf1e9cfdb85eba322c2fa5d66672450aca4adc485054a67
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/friending/RawContactRequestAcceptEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawContactRequestAcceptEvent::RawContactRequestAcceptEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) acceptor,
    TAKE(ApparentSubject) acceptee
) : RawEvent(timestamp, index), acceptor(move(acceptor)), acceptee(move(acceptee)) {
    sanityCheckMandatoryParameters();
}

void RawContactRequestAcceptEvent::sanityCheckMandatoryParameters() const {
    invariant(acceptor, "Parameter 'acceptor' cannot have empty value");
    invariant(acceptee, "Parameter 'acceptee' cannot have empty value");
}

RawEventSubType RawContactRequestAcceptEvent::subType() const {
    return RawEventSubType::CONTACT_REQUEST_ACCEPT;
}

QString RawContactRequestAcceptEvent::eventName() const {
    return "ContactRequestAccept";
}

void RawContactRequestAcceptEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " acceptor=" << acceptor.get() << " acceptee=" << acceptee.get();
}

}}}
