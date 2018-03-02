/**
 * intermediate_format/events/file_transfer/RawStartFileTransferEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 1bbd3e8379c2551a4c825a251be13c0effc09aa1914418d1212276cd7b1c9d59
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/file_transfer/RawStartFileTransferEvent.h"
#include "intermediate_format/subjects/visitor/visit_subject_utils.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawStartFileTransferEvent::RawStartFileTransferEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    IMM(RawTransferredFile) file
) : RawEvent(timestamp, index), file(file) {
    // Nothing else to initialize
}

RawEventSubType RawStartFileTransferEvent::subType() const {
    return RawEventSubType::START_FILE_TRANSFER;
}

bool RawStartFileTransferEvent::visitSubjectsImpl(IMM(visit_subjects_callback_t) callback) const {
    return visit_subjects(sender, callback);
}

bool RawStartFileTransferEvent::alterSubjectsImpl(IMM(alter_subjects_callback_t) callback) {
    return alter_subjects(sender, callback);
}

CEDE(RawStartFileTransferEvent) RawStartFileTransferEvent::deserializeFromStream(
    QDataStream& mut_stream,
    bool skip_type
) {
    maybeDeserializeType(skip_type, mut_stream, RawEventSubType::START_FILE_TRANSFER);

    ApparentTime timestamp = must_deserialize(mut_stream, ApparentTime);
    uint32_t index = must_deserialize(mut_stream, uint32_t);
    optional<QByteArray> skype_guid = must_deserialize(mut_stream, optional<QByteArray>);
    RawTransferredFile file = must_deserialize(mut_stream, RawTransferredFile);
    unique_ptr<ApparentSubject> sender = must_deserialize_optional_unique_ptr(mut_stream, ApparentSubject);

    unique_ptr<RawStartFileTransferEvent> event = make_unique<RawStartFileTransferEvent>(timestamp, index, file);
    event->skypeGUID = skype_guid;
    event->sender = move(sender);

    return event;
}

void RawStartFileTransferEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << file;
    serialize_optional_unique_ptr(mut_stream, sender);
}

void RawStartFileTransferEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " file=" << file;
    if (sender) {
        stream << " sender=" << sender;
    }
}

}}}
