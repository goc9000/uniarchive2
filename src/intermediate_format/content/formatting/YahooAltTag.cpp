/**
 * intermediate_format/content/formatting/YahooAltTag.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 2b3f83fb6e1a62ee5d46bce29da7f895b14af27c798ae9a48980b0999387e32b
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/YahooAltTag.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::utils::serialization;

YahooAltTag::YahooAltTag(IMM(Color) color1, IMM(Color) color2) : StandardTag(true), color1(color1), color2(color2) {
    // Nothing else to initialize
}

YahooAltTag::YahooAltTag() : StandardTag(false) {
    // Nothing else to initialize
}

RawMessageContentItemSubType YahooAltTag::subType() const {
    return RawMessageContentItemSubType::YAHOO_ALT_TAG;
}

QString YahooAltTag::tagName() const {
    return "Alt";
}

bool YahooAltTag::visitSubjectsImpl(IApparentSubjectVisitor& UNUSED visitor) {
    // No fields to visit
    return true;
}

CEDE(YahooAltTag) YahooAltTag::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, RawMessageContentItemSubType::YAHOO_ALT_TAG);

    if (!must_deserialize(mut_stream, bool)) {
        return make_unique<YahooAltTag>();
    }

    Color color1 = must_deserialize(mut_stream, Color);
    Color color2 = must_deserialize(mut_stream, Color);

    unique_ptr<YahooAltTag> item = make_unique<YahooAltTag>(color1, color2);

    return item;
}

void YahooAltTag::serializeOpenTagAttributesToStream(QDataStream& mut_stream) const {
    mut_stream << *color1 << *color2;
}

void YahooAltTag::writeOpenTagAttributesToDebugStream(QDebug stream) const {
    stream << " " << color1 << " " << color2;
}

}}}
