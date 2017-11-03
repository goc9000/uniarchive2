/**
 * intermediate_format/events/system/RawEEEncryptionAnnouncementEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 51dba92e177c05e2942497d1edace7e5aa552c647d16714fac3679cea9d21d7d
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/system/RawEEEncryptionAnnouncementEvent.h"
#include "utils/serialization/deserialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawEEEncryptionAnnouncementEvent::RawEEEncryptionAnnouncementEvent(IMM(ApparentTime) timestamp, uint32_t index)
  : RawEvent(timestamp, index) {
    // Nothing else to initialize
}

RawEventSubType RawEEEncryptionAnnouncementEvent::subType() const {
    return RawEventSubType::EE_ENCRYPTION_ANNOUNCEMENT;
}

bool RawEEEncryptionAnnouncementEvent::visitSubjectsImpl(IApparentSubjectVisitor& UNUSED visitor) {
    // No fields to visit
    return true;
}

CEDE(RawEEEncryptionAnnouncementEvent) RawEEEncryptionAnnouncementEvent::deserializeFromStream(
    QDataStream& mut_stream,
    bool skip_type
) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::EE_ENCRYPTION_ANNOUNCEMENT);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);

    unique_ptr<RawEEEncryptionAnnouncementEvent> event = make_unique<RawEEEncryptionAnnouncementEvent>(
        timestamp,
        index
    );
    event->skypeGUID = skype_guid;

    return event;
}

void RawEEEncryptionAnnouncementEvent::serializeDetailsToStream(QDataStream& UNUSED mut_stream) const {
    // No fields to serialize
}

void RawEEEncryptionAnnouncementEvent::writeDetailsToDebugStream(QDebug UNUSED stream) const {
    // No fields to write
}

}}}
