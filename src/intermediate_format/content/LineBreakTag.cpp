/**
 * intermediate_format/content/LineBreakTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/LineBreakTag.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace content {

LineBreakTag::LineBreakTag() {
}

void LineBreakTag::writeToDebugStream(QDebug stream) const {
    stream << "[BRK]";
}

}}}
