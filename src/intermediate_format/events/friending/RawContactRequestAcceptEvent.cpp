/**
 * intermediate_format/events/friending/RawContactRequestAcceptEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 8503627b97cdbfc2896001dc56334995cd3e143bf71a71ed034cf8f229ff3ce1
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/friending/RawContactRequestAcceptEvent.h"
#include "intermediate_format/subjects/visitor/visit_subject_utils.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawContactRequestAcceptEvent::RawContactRequestAcceptEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) acceptor,
    TAKE(ApparentSubject) acceptee
) : RawEvent(timestamp, index), acceptor(move(acceptor)), acceptee(move(acceptee)) {
    sanityCheckMandatoryParameters();
}

void RawContactRequestAcceptEvent::sanityCheckMandatoryParameters() const {
    invariant(acceptor, "Parameter 'acceptor' cannot have empty value");
    invariant(acceptee, "Parameter 'acceptee' cannot have empty value");
}

RawEventSubType RawContactRequestAcceptEvent::subType() const {
    return RawEventSubType::CONTACT_REQUEST_ACCEPT;
}

bool RawContactRequestAcceptEvent::visitSubjectsImpl(IApparentSubjectVisitor& visitor) {
    return visit_subjects(acceptor, visitor) && visit_subjects(acceptee, visitor);
}

CEDE(RawContactRequestAcceptEvent) RawContactRequestAcceptEvent::deserializeFromStream(
    QDataStream& mut_stream,
    bool skip_type
) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::CONTACT_REQUEST_ACCEPT);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    unique_ptr<ApparentSubject> acceptor = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);
    unique_ptr<ApparentSubject> acceptee = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);

    unique_ptr<RawContactRequestAcceptEvent> event = make_unique<RawContactRequestAcceptEvent>(
        timestamp,
        index,
        move(acceptor),
        move(acceptee)
    );
    event->skypeGUID = skype_guid;

    return event;
}

void RawContactRequestAcceptEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << acceptor << acceptee;
}

void RawContactRequestAcceptEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " acceptor=" << acceptor << " acceptee=" << acceptee;
}

}}}
