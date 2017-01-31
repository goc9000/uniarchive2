/**
 * intermediate_format/content/LineBreak.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 784604b84fd09be1859a1f48079f15400d225494f2fa0608c4016716e0561c07
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/LineBreak.h"
#include "utils/serialization/deserialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::utils::serialization;

LineBreak::LineBreak() : RawMessageContentItem() {
    // Nothing else to initialize
}

RawMessageContentItemSubType LineBreak::subType() const {
    return RawMessageContentItemSubType::LINE_BREAK;
}

CEDE(LineBreak) LineBreak::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawMessageContentItemSubType::LINE_BREAK);

    unique_ptr<LineBreak> item = make_unique<LineBreak>();

    return item;
}

void LineBreak::serializeToStreamImpl(QDataStream& UNUSED mut_stream) const {
    // No fields to serialize
}

void LineBreak::writeToDebugStreamImpl(QDebug stream) const {
    stream << "[BRK]";
}

}}}
