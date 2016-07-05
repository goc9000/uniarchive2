/**
 * intermediate_format/content/formatting/ANSIColorTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/ANSIColorTag.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace content {

ANSIColorTag::ANSIColorTag(ANSIColor color, bool closed) : color(color), closed(closed) {
}

void ANSIColorTag::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);

    stream.nospace() << "[" << (closed ? "/" : "") << "ANSICol:" << color << "]";
}

}}}
