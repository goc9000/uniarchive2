/**
 * intermediate_format/content/StandardTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/StandardTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

StandardTag::StandardTag(bool open) : AbstractTag(), open(open) {
}

bool StandardTag::isClosed() const {
    return !open;
}

void StandardTag::writeAttributesToDebugStream(QDebug stream) const {
    if (open) {
        writeOpenTagAttributesToDebugStream(stream);
    }
}

void StandardTag::writeOpenTagAttributesToDebugStream(QDebug UNUSED stream) const {
}

}}}
