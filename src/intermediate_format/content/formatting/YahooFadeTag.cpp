/**
 * intermediate_format/content/formatting/YahooFadeTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/YahooFadeTag.h"
#include "utils/language/invariant.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

YahooFadeTag::YahooFadeTag(bool closed, IMM(QList<Color>) colors)
    : colors(colors), closed(closed) {
}

void YahooFadeTag::writeToDebugStreamImpl(QDebug stream) const {
    stream << "[" << (closed ? "/" : "") << "Fade";
    if (!closed && !colors.isEmpty()) {
        stream << ":" << colors[0];
        if (colors.length() > 1) {
            stream << "," << colors[1];
        }
        if (colors.length() > 2) {
            stream << "+";
        }
    }
    stream << "]";
}

}}}
