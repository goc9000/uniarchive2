/**
 * intermediate_format/content/formatting/UnderlineTag.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): dbd2fb69f081aaf73bdbe66a9008bf6e1d6200afc23e7127a0cdfef112bf3071
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/UnderlineTag.h"
#include "utils/serialization/deserialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::utils::serialization;

UnderlineTag::UnderlineTag(bool open) : StandardTag(open) {
    // Nothing else to initialize
}

RawMessageContentItemSubType UnderlineTag::subType() const {
    return RawMessageContentItemSubType::UNDERLINE_TAG;
}

QString UnderlineTag::tagName() const {
    return "U";
}

bool UnderlineTag::visitSubjectsImpl(IMM(visit_subjects_callback_t) UNUSED callback) const {
    // No fields to visit
    return true;
}

bool UnderlineTag::alterSubjectsImpl(IMM(alter_subjects_callback_t) UNUSED callback) {
    // No fields to visit
    return true;
}

CEDE(UnderlineTag) UnderlineTag::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawMessageContentItemSubType::UNDERLINE_TAG);

    if (!must_deserialize(mut_stream, bool)) {
        return make_unique<UnderlineTag>(false);
    }

    unique_ptr<UnderlineTag> item = make_unique<UnderlineTag>(true);

    return item;
}

void UnderlineTag::serializeOpenTagAttributesToStream(QDataStream& UNUSED mut_stream) const {
    // No fields to serialize
}

void UnderlineTag::writeOpenTagAttributesToDebugStream(QDebug UNUSED stream) const {
    // No fields to write
}

}}}
