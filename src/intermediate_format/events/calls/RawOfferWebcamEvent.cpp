/**
 * intermediate_format/events/calls/RawOfferWebcamEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): dc2f645e15a54daef77cb61c21247c57ef9dd5a838349af7c2741d46379c7ff6
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/calls/RawOfferWebcamEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

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

void RawOfferWebcamEvent::serializeFailableEventDetailsToStream(QDataStream& mut_stream) const {
    // TODO: add field serialization code
}

void RawOfferWebcamEvent::writeFailableEventDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender.get();
}

}}}
