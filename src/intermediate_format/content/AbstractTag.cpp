/**
 * intermediate_format/content/AbstractTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/AbstractTag.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

AbstractTag::AbstractTag() : RawMessageContentItem() {
    // Nothing else to initialize
}

void AbstractTag::writeToDebugStreamImpl(QDebug stream) const {
    stream << "[" << (isClosed() ? "/" : "") << QP(tagName());
    writeAttributesToDebugStream(stream);
    stream << "]";
}

}}}
