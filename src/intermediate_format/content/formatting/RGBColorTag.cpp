/**
 * intermediate_format/content/formatting/RGBColorTag.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 8b9869294e7e3d9252eddcdc18d004e1ec481a275c6230b45dfa13fc211667e6
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/RGBColorTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

RGBColorTag::RGBColorTag(bool open, IMM(Color) color) : SymmetricTag(open), color(color) {
}

RawMessageContentItemSubType RGBColorTag::subType() const {
    return RawMessageContentItemSubType::RGB_COLOR_TAG;
}

QString RGBColorTag::tagName() const {
    return "RGBCol";
}

void RGBColorTag::writeAttributesToDebugStream(QDebug stream) const {
    stream << " " << color;
}

}}}
