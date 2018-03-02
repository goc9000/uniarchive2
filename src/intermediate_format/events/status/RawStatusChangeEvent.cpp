/**
 * intermediate_format/events/status/RawStatusChangeEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 8760d9f36b0fe2695e6885d1c92276341bae228584d9ecf98641e324346573eb
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/status/RawStatusChangeEvent.h"
#include "intermediate_format/subjects/visitor/visit_subject_utils.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawStatusChangeEvent::RawStatusChangeEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) subject,
    IMStatus status
) : RawEvent(timestamp, index), subject(move(subject)), status(status) {
    sanityCheckMandatoryParameters();
}

void RawStatusChangeEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
}

RawEventSubType RawStatusChangeEvent::subType() const {
    return RawEventSubType::STATUS_CHANGE;
}

bool RawStatusChangeEvent::visitSubjectsImpl(IMM(visit_subjects_callback_t) callback) const {
    return visit_subjects(subject, callback) && visit_subjects(message, callback);
}

bool RawStatusChangeEvent::alterSubjectsImpl(IMM(alter_subjects_callback_t) callback) {
    return alter_subjects(subject, callback) && alter_subjects(message, callback);
}

CEDE(RawStatusChangeEvent) RawStatusChangeEvent::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::STATUS_CHANGE);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    unique_ptr<ApparentSubject> subject = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);
    IMStatus status = must_deserialize(mut_stream, IMStatus);
    optional<RawMessageContent> message = must_deserialize(mut_stream, optional<RawMessageContent>);

    unique_ptr<RawStatusChangeEvent> event = make_unique<RawStatusChangeEvent>(timestamp, index, move(subject), status);
    event->skypeGUID = skype_guid;
    event->message = move(message);

    return event;
}

void RawStatusChangeEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << subject << status << message;
}

void RawStatusChangeEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject << " status=" << status;
    if (message) {
        stream << " message=" << message;
    }
}

}}}
