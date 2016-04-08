/**
 * intermediate_format/content/ANSIColorTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/content/ANSIColorTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

ANSIColorTag::ANSIColorTag(ANSIColors color, bool closed) : color(color), closed(closed) {
}

void ANSIColorTag::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);

    stream.nospace() << "[" << (closed ? "/" : "") << "ANSICol:" << color << "]";
}

}}}
