/**
 * intermediate_format/events/conference/RawRemoveFromConferenceEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 2921f1081dc1e588aba7c89f5c63467d4e974217407e61fe6857fe1b7184b7e3
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawRemoveFromConferenceEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawRemoveFromConferenceEvent::RawRemoveFromConferenceEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) removee
) : RawEvent(timestamp, index), removee(move(removee)) {
    sanityCheckMandatoryParameters();
}

RawRemoveFromConferenceEvent::RawRemoveFromConferenceEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) remover,
    TAKE(ApparentSubject) removee
) : RawEvent(timestamp, index), remover(move(remover)), removee(move(removee)) {
    sanityCheckMandatoryParameters();
    invariant(this->remover, "Parameter 'remover' cannot have empty value");
}

void RawRemoveFromConferenceEvent::sanityCheckMandatoryParameters() const {
    invariant(removee, "Parameter 'removee' cannot have empty value");
}

RawEventSubType RawRemoveFromConferenceEvent::subType() const {
    return RawEventSubType::REMOVE_FROM_CONFERENCE;
}

void RawRemoveFromConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (remover) {
        stream << " remover=" << remover.get();
    }
    stream << " removee=" << removee.get();
}

}}}
