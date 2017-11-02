/**
 * intermediate_format/events/calls/RawOfferCallEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): ddc9c66580b654da22337ab1a729cb7289acbe9fa1d4c921bed0344d5cb70fc5
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/calls/RawOfferCallEvent.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

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

bool RawOfferCallEvent::visitSubjectsImpl(IApparentSubjectVisitor& UNUSED visitor) {
    return true;
}

CEDE(RawOfferCallEvent) RawOfferCallEvent::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::OFFER_CALL);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    unique_ptr<ApparentSubject> caller = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);

    unique_ptr<RawOfferCallEvent> event = make_unique<RawOfferCallEvent>(timestamp, index, move(caller));
    event->skypeGUID = skype_guid;

    return event;
}

void RawOfferCallEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << caller;
}

void RawOfferCallEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " caller=" << caller;
}

}}}
