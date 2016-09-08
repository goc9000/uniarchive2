/**
 * intermediate_format/content/formatting/RGBColorTag.cpp
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

QString RGBColorTag::tagName() const {
    return "RGBCol";
}

void RGBColorTag::writeAttributesToDebugStream(QDebug stream) const {
    stream << " " << color;
}

}}}
