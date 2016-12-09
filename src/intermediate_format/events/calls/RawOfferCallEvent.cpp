/**
 * intermediate_format/events/calls/RawOfferCallEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 894a05efe421ee9b8e1b40d2fb9116d5e14a127c4ae5c473d92eaf488805dc4d
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/calls/RawOfferCallEvent.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawOfferCallEvent::RawOfferCallEvent(IMM(ApparentTime) timestamp, uint32_t index, TAKE(ApparentSubject) caller)
  : RawEvent(timestamp, index), caller(move(caller)) {
    sanityCheckMandatoryParameters();
}

void RawOfferCallEvent::sanityCheckMandatoryParameters() const {
    invariant(caller, "Parameter 'caller' cannot have empty value");
}

RawEventSubType RawOfferCallEvent::subType() const {
    return RawEventSubType::OFFER_CALL;
}

void RawOfferCallEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << caller;
}

void RawOfferCallEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " caller=" << caller;
}

}}}
