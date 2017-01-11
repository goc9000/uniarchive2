/**
 * intermediate_format/content/formatting/FontTag.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): bd9f2ca0e597802427da84d3c8f9f86795b791588ef4c63405619e2e352af327
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/FontTag.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::utils::serialization;

FontTag::FontTag(bool open) : StandardTag(open) {
}

RawMessageContentItemSubType FontTag::subType() const {
    return RawMessageContentItemSubType::FONT_TAG;
}

QString FontTag::tagName() const {
    return "Font";
}

CEDE(FontTag) FontTag::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawMessageContentItemSubType::FONT_TAG);

    if (!must_deserialize(mut_stream, bool)) {
        return make_unique<FontTag>(false);
    }

    optional<vector<QString>> faces = must_deserialize(mut_stream, optional<vector<QString>>);
    optional<QString> size = must_deserialize(mut_stream, optional<QString>);
    optional<Color> color = must_deserialize(mut_stream, optional<Color>);
    optional<QString> css = must_deserialize(mut_stream, optional<QString>);

    unique_ptr<FontTag> item = make_unique<FontTag>(true);
    item->faces = faces;
    item->size = size;
    item->color = color;
    item->css = css;

    return item;
}

void FontTag::serializeOpenTagAttributesToStream(QDataStream& mut_stream) const {
    mut_stream << faces << size << color << css;
}

void FontTag::writeOpenTagAttributesToDebugStream(QDebug stream) const {
    if (faces) {
        if (faces->size() == 1) {
            stream << " face=" << faces->front();
        } else {
            stream << " faces=" << faces;
        }
    }
    if (size) {
        stream << " size=" << size;
    }
    if (color) {
        stream << " color=" << color;
    }
    if (css) {
        stream << " css=" << css;
    }
}

}}}
