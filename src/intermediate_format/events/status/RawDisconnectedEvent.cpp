/**
 * intermediate_format/events/status/RawDisconnectedEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): f211b8190531f1869dc4e480768595d27d8dd41eef89972baa8182cdb39545c0
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/status/RawDisconnectedEvent.h"
#include "intermediate_format/subjects/visitor/visit_subject_utils.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawDisconnectedEvent::RawDisconnectedEvent(IMM(ApparentTime) timestamp, uint32_t index, TAKE(ApparentSubject) subject)
  : RawEvent(timestamp, index), subject(move(subject)) {
    sanityCheckMandatoryParameters();
}

void RawDisconnectedEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
}

RawEventSubType RawDisconnectedEvent::subType() const {
    return RawEventSubType::DISCONNECTED;
}

bool RawDisconnectedEvent::visitSubjectsImpl(IMM(visit_subjects_callback_t) callback) {
    return visit_subjects(subject, callback);
}

CEDE(RawDisconnectedEvent) RawDisconnectedEvent::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::DISCONNECTED);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    unique_ptr<ApparentSubject> subject = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);

    unique_ptr<RawDisconnectedEvent> event = make_unique<RawDisconnectedEvent>(timestamp, index, move(subject));
    event->skypeGUID = skype_guid;

    return event;
}

void RawDisconnectedEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << subject;
}

void RawDisconnectedEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject;
}

}}}
