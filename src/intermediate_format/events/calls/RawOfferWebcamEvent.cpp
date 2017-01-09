/**
 * intermediate_format/events/calls/RawOfferWebcamEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 55694227608b0b33db9d016e80751da335b6be0850d5359bde890e872035bc8d
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/calls/RawOfferWebcamEvent.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawOfferWebcamEvent::RawOfferWebcamEvent(IMM(ApparentTime) timestamp, uint32_t index, TAKE(ApparentSubject) sender)
  : RawFailableEvent(timestamp, index), sender(move(sender)) {
    sanityCheckMandatoryParameters();
}

void RawOfferWebcamEvent::sanityCheckMandatoryParameters() const {
    invariant(sender, "Parameter 'sender' cannot have empty value");
}

RawEventSubType RawOfferWebcamEvent::subType() const {
    return RawEventSubType::OFFER_WEBCAM;
}

CEDE(RawOfferWebcamEvent) RawOfferWebcamEvent::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    invariant_violation("RawOfferWebcamEvent deserialization not yet implemented");
}

void RawOfferWebcamEvent::serializeFailableEventDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << sender;
}

void RawOfferWebcamEvent::writeFailableEventDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender;
}

}}}
