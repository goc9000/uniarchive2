/**
 * intermediate_format/events/status/RawConnectedEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 90095dd34267df32e1fdb79a3f9fde9fa88fbb40b5b2702e7a16271aa8a37828
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/status/RawConnectedEvent.h"
#include "intermediate_format/subjects/visitor/visit_subject_utils.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawConnectedEvent::RawConnectedEvent(IMM(ApparentTime) timestamp, uint32_t index, TAKE(ApparentSubject) subject)
  : RawEvent(timestamp, index), subject(move(subject)) {
    sanityCheckMandatoryParameters();
}

void RawConnectedEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
}

RawEventSubType RawConnectedEvent::subType() const {
    return RawEventSubType::CONNECTED;
}

bool RawConnectedEvent::visitSubjectsImpl(IApparentSubjectVisitor& visitor) {
    return visit_subjects(subject, visitor);
}

CEDE(RawConnectedEvent) RawConnectedEvent::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::CONNECTED);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    unique_ptr<ApparentSubject> subject = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);

    unique_ptr<RawConnectedEvent> event = make_unique<RawConnectedEvent>(timestamp, index, move(subject));
    event->skypeGUID = skype_guid;

    return event;
}

void RawConnectedEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << subject;
}

void RawConnectedEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject;
}

}}}
