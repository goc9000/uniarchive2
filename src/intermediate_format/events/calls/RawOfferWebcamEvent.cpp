/**
 * intermediate_format/events/calls/RawOfferWebcamEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 483ca543ecb983fcb3233710a18d92ffce8013765ad7c7fa6ebced00efb1535e
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
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::OFFER_WEBCAM);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    optional<OfferWebcamFailReason> fail_reason = must_deserialize(mut_stream, optional<OfferWebcamFailReason>);
    unique_ptr<ApparentSubject> sender = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);

    unique_ptr<RawOfferWebcamEvent> event = make_unique<RawOfferWebcamEvent>(timestamp, index, move(sender));
    event->reasonFailed = fail_reason;
    event->skypeGUID = skype_guid;

    return event;
}

void RawOfferWebcamEvent::serializeFailableEventDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << sender;
}

void RawOfferWebcamEvent::writeFailableEventDetailsToDebugStream(QDebug stream) const {
    stream << " sender=" << sender;
}

}}}
