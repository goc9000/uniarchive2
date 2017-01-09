/**
 * intermediate_format/events/file_transfer/RawStartFileTransferEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 674c6cc500db61c1b6cc2e10936a9240b5e97390ed30e9f9fe1a6be7de3e04d4
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
    invariant_violation("RawStartFileTransferEvent deserialization not yet implemented");
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
