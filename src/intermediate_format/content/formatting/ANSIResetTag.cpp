/**
 * intermediate_format/content/formatting/ANSIResetTag.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): aa9609fad6f9cad8fd9d8eb1e13830765f88935e615fb5dec1a547d3a044f649
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/ANSIResetTag.h"
#include "utils/serialization/deserialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::utils::serialization;

ANSIResetTag::ANSIResetTag() : SelfClosingTag() {
}

RawMessageContentItemSubType ANSIResetTag::subType() const {
    return RawMessageContentItemSubType::ANSI_RESET_TAG;
}

QString ANSIResetTag::tagName() const {
    return "ANSIReset";
}

CEDE(ANSIResetTag) ANSIResetTag::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawMessageContentItemSubType::ANSI_RESET_TAG);

    unique_ptr<ANSIResetTag> item = make_unique<ANSIResetTag>();

    return item;
}

void ANSIResetTag::serializeAttributesToStream(QDataStream& UNUSED mut_stream) const {
}

}}}
