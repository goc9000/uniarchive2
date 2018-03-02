/**
 * intermediate_format/events/conference/RawRejoinConferenceEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): e4336850c684eb90a8529b928ea03a9d876ffbfeb30ab9fb2f8a0671186adf04
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawRejoinConferenceEvent.h"
#include "intermediate_format/subjects/visitor/visit_subject_utils.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawRejoinConferenceEvent::RawRejoinConferenceEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) subject,
    TAKE_VEC(ApparentSubject) peers
) : RawEvent(timestamp, index), subject(move(subject)), peers(move(peers)) {
    sanityCheckMandatoryParameters();
}

void RawRejoinConferenceEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
    for (IMM(auto) item : peers) {
        invariant(item, "Parameter 'peers' cannot have empty entries");
    }
}

RawEventSubType RawRejoinConferenceEvent::subType() const {
    return RawEventSubType::REJOIN_CONFERENCE;
}

bool RawRejoinConferenceEvent::visitSubjectsImpl(IMM(visit_subjects_callback_t) callback) {
    return visit_subjects(subject, callback) && visit_subjects(peers, callback);
}

CEDE(RawRejoinConferenceEvent) RawRejoinConferenceEvent::deserializeFromStream(
    QDataStream& mut_stream,
    bool skip_type
) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::REJOIN_CONFERENCE);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    unique_ptr<ApparentSubject> subject = must_deserialize(mut_stream, unique_ptr<ApparentSubject>);
    vector<unique_ptr<ApparentSubject>> peers = must_deserialize(mut_stream, vector<unique_ptr<ApparentSubject>>);

    unique_ptr<RawRejoinConferenceEvent> event = make_unique<RawRejoinConferenceEvent>(
        timestamp,
        index,
        move(subject),
        move(peers)
    );
    event->skypeGUID = skype_guid;

    return event;
}

void RawRejoinConferenceEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << subject << peers;
}

void RawRejoinConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject << " peers=" << peers;
}

}}}
