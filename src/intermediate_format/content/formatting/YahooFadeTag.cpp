/**
 * intermediate_format/content/formatting/YahooFadeTag.cpp
 *
 * WARNING: This file is partially autogenerated. Edit only the custom sections, or autogen_config.yml and re-generate.
 * Codegen hash (sha256): f68c7bcce7602aa2c36cacd95a10907e5e0e05592d605423a69288c93c219282
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/YahooFadeTag.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::utils::serialization;

YahooFadeTag::YahooFadeTag(bool open) : StandardTag(open) {
    // Nothing else to initialize
}

YahooFadeTag::YahooFadeTag(IMM(vector<Color>) colors) : StandardTag(true), colors(colors) {
    // Nothing else to initialize
}

RawMessageContentItemSubType YahooFadeTag::subType() const {
    return RawMessageContentItemSubType::YAHOO_FADE_TAG;
}

QString YahooFadeTag::tagName() const {
    return "Fade";
}

bool YahooFadeTag::visitSubjectsImpl(IMM(visit_subjects_callback_t) UNUSED callback) {
    // No fields to visit
    return true;
}

CEDE(YahooFadeTag) YahooFadeTag::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawMessageContentItemSubType::YAHOO_FADE_TAG);

    if (!must_deserialize(mut_stream, bool)) {
        return make_unique<YahooFadeTag>(false);
    }

    optional<vector<Color>> colors = must_deserialize(mut_stream, optional<vector<Color>>);

    unique_ptr<YahooFadeTag> item = make_unique<YahooFadeTag>(true);
    item->colors = colors;

    return item;
}

void YahooFadeTag::serializeOpenTagAttributesToStream(QDataStream& mut_stream) const {
    mut_stream << colors;
}

void YahooFadeTag::writeOpenTagAttributesToDebugStream(QDebug stream) const {
    // BEGIN CUSTOM SECTION: Debug write method
    if (colors) {
        stream << " " << colors->at(0);
        if (colors->size() > 1) {
            stream << "," << colors->at(1);
        }
        if (colors->size() > 2) {
            stream << "...";
        }
    }
    // END CUSTOM SECTION: Debug write method
}

}}}
