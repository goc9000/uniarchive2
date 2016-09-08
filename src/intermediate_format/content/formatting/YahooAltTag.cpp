/**
 * intermediate_format/content/formatting/YahooAltTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/YahooAltTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

YahooAltTag::YahooAltTag(IMM(Color) color1, IMM(Color) color2) : StandardTag(true), color1(color1), color2(color2) {
}

YahooAltTag::YahooAltTag() : StandardTag(false) {
}

QString YahooAltTag::tagName() const {
    return "Alt";
}

void YahooAltTag::writeOpenTagAttributesToDebugStream(QDebug stream) const {
    stream << " " << *color1 << " " << *color2;
}

}}}
