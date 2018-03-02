/**
 * intermediate_format/events/friending/RawContactRequestAcceptEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): f8678a00b06c292ddfc226ae59efb97c49a29877c30fed1fb4ad78ab4e62e4bc
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

bool RawContactRequestAcceptEvent::visitSubjectsImpl(IMM(visit_subjects_callback_t) callback) const {
    return visit_subjects(acceptor, callback) && visit_subjects(acceptee, callback);
}

bool RawContactRequestAcceptEvent::alterSubjectsImpl(IMM(alter_subjects_callback_t) callback) {
    return alter_subjects(acceptor, callback) && alter_subjects(acceptee, callback);
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
