/**
 * intermediate_format/events/conference/RawDeclineConferenceEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 8c8aadc7bf6cafb1e678f02b0b386811ec6b2475d4d7df2aa9207a673a49e740
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawDeclineConferenceEvent.h"
#include "intermediate_format/subjects/visitor/visit_subject_utils.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawDeclineConferenceEvent::RawDeclineConferenceEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) subject
) : RawEvent(timestamp, index), subject(move(subject)) {
    sanityCheckMandatoryParameters();
}

void RawDeclineConferenceEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
}

RawEventSubType RawDeclineConferenceEvent::subType() const {
    return RawEventSubType::DECLINE_CONFERENCE;
}

bool RawDeclineConferenceEvent::visitSubjectsImpl(IMM(visit_subjects_callback_t) callback) const {
    return visit_subjects(subject, callback) && visit_subjects(message, callback);
}

bool RawDeclineConferenceEvent::alterSubjectsImpl(IMM(alter_subjects_callback_t) callback) {
    return alter_subjects(subject, callback) && alter_subjects(message, callback);
}

CEDE(RawDeclineConferenceEvent) RawDeclineConferenceEvent::deserializeFromStream(
    QDataStream& mut_stream,
    bool skip_type
) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::DECLINE_CONFERENCE);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    unique_ptr<ApparentSubject> subject = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);
    optional<RawMessageContent> message = must_deserialize(mut_stream, optional<RawMessageContent>);

    unique_ptr<RawDeclineConferenceEvent> event = make_unique<RawDeclineConferenceEvent>(
        timestamp,
        index,
        move(subject)
    );
    event->skypeGUID = skype_guid;
    event->message = move(message);

    return event;
}

void RawDeclineConferenceEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << subject << message;
}

void RawDeclineConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject;
    if (message) {
        stream << " message=" << message;
    }
}

}}}
