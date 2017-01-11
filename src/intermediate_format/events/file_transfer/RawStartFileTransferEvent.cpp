/**
 * intermediate_format/events/file_transfer/RawStartFileTransferEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): a1ba15d75932425d9444a9177309503a09faa023965c532c5e1d06af16234c5a
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/file_transfer/RawStartFileTransferEvent.h"
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
}

RawEventSubType RawStartFileTransferEvent::subType() const {
    return RawEventSubType::START_FILE_TRANSFER;
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
