/**
 * intermediate_format/content/YahooFadeTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/YahooFadeTag.h"
#include "utils/language/invariant.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace content {

YahooFadeTag::YahooFadeTag(bool closed, IMM(QList<Color>) colors)
    : closed(closed), colors(colors) {
}

void YahooFadeTag::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);

    stream.nospace() << "[" << (closed ? "/" : "") << "Fade";
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
