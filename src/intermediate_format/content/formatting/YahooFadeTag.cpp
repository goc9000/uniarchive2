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

namespace uniarchive2 { namespace intermediate_format { namespace content {

YahooFadeTag::YahooFadeTag(IMM(vector<Color>) colors) : StandardTag(true), colors(colors) {
}

YahooFadeTag::YahooFadeTag(bool open) : StandardTag(open) {
}

QString YahooFadeTag::tagName() const {
    return "Fade";
}

void YahooFadeTag::writeOpenTagAttributesToDebugStream(QDebug stream) const {
    if (colors) {
        stream << " " << colors->at(0);
        if (colors->size() > 1) {
            stream << "," << colors->at(1);
        }
        if (colors->size() > 2) {
            stream << "...";
        }
    }
}

}}}
