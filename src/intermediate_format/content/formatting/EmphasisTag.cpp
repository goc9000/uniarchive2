/**
 * intermediate_format/content/formatting/EmphasisTag.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 12969f035df0c18bdf98f632ba04e7ee33f92dc53b31adf98178513cbddbc851
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/EmphasisTag.h"
#include "utils/serialization/deserialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::utils::serialization;

EmphasisTag::EmphasisTag(bool open) : StandardTag(open) {
    // Nothing else to initialize
}

RawMessageContentItemSubType EmphasisTag::subType() const {
    return RawMessageContentItemSubType::EMPHASIS_TAG;
}

QString EmphasisTag::tagName() const {
    return "EM";
}

bool EmphasisTag::visitSubjectsImpl(IMM(visit_subjects_callback_t) UNUSED callback) const {
    // No fields to visit
    return true;
}

bool EmphasisTag::alterSubjectsImpl(IMM(alter_subjects_callback_t) UNUSED callback) {
    // No fields to visit
    return true;
}

CEDE(EmphasisTag) EmphasisTag::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawMessageContentItemSubType::EMPHASIS_TAG);

    if (!must_deserialize(mut_stream, bool)) {
        return make_unique<EmphasisTag>(false);
    }

    unique_ptr<EmphasisTag> item = make_unique<EmphasisTag>(true);

    return item;
}

void EmphasisTag::serializeOpenTagAttributesToStream(QDataStream& UNUSED mut_stream) const {
    // No fields to serialize
}

void EmphasisTag::writeOpenTagAttributesToDebugStream(QDebug UNUSED stream) const {
    // No fields to write
}

}}}
