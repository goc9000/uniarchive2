/**
 * intermediate_format/content/YahooAltTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/content/YahooAltTag.h"
#include "utils/language/invariant.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

YahooAltTag::YahooAltTag(bool closed, IMM(Color) color1, IMM(Color) color2)
    : closed(closed), color1(color1), color2(color2) {
}

YahooAltTag::YahooAltTag(bool closed) : closed(closed) {
    invariant(closed, "Can't instantiate ALT tag without any colors");
}

void YahooAltTag::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);

    stream.nospace() << "[" << (closed ? "/" : "") << "Alt";
    if (!closed) {
        stream << ":" << *color1 << "," << *color2;
    }
    stream << "]";
}

}}}
