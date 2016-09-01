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
#include "utils/language/invariant.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

YahooAltTag::YahooAltTag(bool closed, IMM(Color) color1, IMM(Color) color2)
    : color1(color1), color2(color2), closed(closed) {
}

YahooAltTag::YahooAltTag(bool closed) : closed(closed) {
    invariant(closed, "Can't instantiate ALT tag without any colors");
}

void YahooAltTag::writeToDebugStreamImpl(QDebug stream) const {
    stream << "[" << (closed ? "/" : "") << "Alt";
    if (!closed) {
        stream << ":" << *color1 << "," << *color2;
    }
    stream << "]";
}

}}}
