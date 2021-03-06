/**
 * intermediate_format/content/formatting/RGBColorTag.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 4ce54156ae78008fffcd335bd80255d3f6dc8ffb2ee145c7792d607f588ac8ba
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/RGBColorTag.h"
#include "utils/serialization/deserialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::utils::serialization;

RGBColorTag::RGBColorTag(bool open, IMM(Color) color) : SymmetricTag(open), color(color) {
    // Nothing else to initialize
}

RawMessageContentItemSubType RGBColorTag::subType() const {
    return RawMessageContentItemSubType::RGB_COLOR_TAG;
}

QString RGBColorTag::tagName() const {
    return "RGBCol";
}

bool RGBColorTag::visitSubjectsImpl(IMM(visit_subjects_callback_t) UNUSED callback) const {
    // No fields to visit
    return true;
}

bool RGBColorTag::alterSubjectsImpl(IMM(alter_subjects_callback_t) UNUSED callback) {
    // No fields to visit
    return true;
}

CEDE(RGBColorTag) RGBColorTag::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawMessageContentItemSubType::RGB_COLOR_TAG);

    bool open = must_deserialize(mut_stream, bool);
    Color color = must_deserialize(mut_stream, Color);

    unique_ptr<RGBColorTag> item = make_unique<RGBColorTag>(open, color);

    return item;
}

void RGBColorTag::serializeAttributesToStream(QDataStream& mut_stream) const {
    mut_stream << color;
}

void RGBColorTag::writeAttributesToDebugStream(QDebug stream) const {
    stream << " " << color;
}

}}}
