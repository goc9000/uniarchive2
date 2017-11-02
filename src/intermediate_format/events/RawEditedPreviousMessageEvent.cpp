/**
 * intermediate_format/events/RawEditedPreviousMessageEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): c43d8e93f48cb8127ec5968cd882839c0eed2ed4af330cb6143291118c7ce1b7
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawEditedPreviousMessageEvent.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawEditedPreviousMessageEvent::RawEditedPreviousMessageEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) subject
) : RawEvent(timestamp, index), subject(move(subject)) {
    sanityCheckMandatoryParameters();
}

void RawEditedPreviousMessageEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
}

RawEventSubType RawEditedPreviousMessageEvent::subType() const {
    return RawEventSubType::EDITED_PREVIOUS_MESSAGE;
}

void RawEditedPreviousMessageEvent::visitSubjectsImpl(ApparentSubjectVisitor& UNUSED visitor) {
    // No fields to visit
}

CEDE(RawEditedPreviousMessageEvent) RawEditedPreviousMessageEvent::deserializeFromStream(
    QDataStream& mut_stream,
    bool skip_type
) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::EDITED_PREVIOUS_MESSAGE);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    unique_ptr<ApparentSubject> subject = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);

    unique_ptr<RawEditedPreviousMessageEvent> event = make_unique<RawEditedPreviousMessageEvent>(
        timestamp,
        index,
        move(subject)
    );
    event->skypeGUID = skype_guid;

    return event;
}

void RawEditedPreviousMessageEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << subject;
}

void RawEditedPreviousMessageEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject;
}

}}}
