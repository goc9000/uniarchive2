/**
 * intermediate_format/content/LineBreak.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/LineBreak.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

LineBreak::LineBreak() : RawMessageContentItem() {
}

void LineBreak::writeToDebugStreamImpl(QDebug stream) const {
    stream << "[BRK]";
}

}}}
