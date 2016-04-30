/**
 * intermediate_format/content/YahooFadeTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/content/YahooFadeTag.h"
#include "utils/language/invariant.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

YahooFadeTag::YahooFadeTag(bool closed, IMM(QList<Color>) colors)
    : closed(closed), colors(colors) {
    invariant(closed || !colors.isEmpty(), "Can't instantiate FADE tag without any colors");
}

void YahooFadeTag::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);

    stream.nospace() << "[" << (closed ? "/" : "") << "Fade";
    if (!closed) {
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
