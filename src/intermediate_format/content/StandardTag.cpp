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
    // Nothing else to initialize
}

bool StandardTag::isClosed() const {
    return !open;
}

void StandardTag::serializeToStreamImpl(QDataStream& mut_stream) const {
    mut_stream << open;

    if (open) {
        serializeOpenTagAttributesToStream(mut_stream);
    }
}

void StandardTag::writeAttributesToDebugStream(QDebug stream) const {
    if (open) {
        writeOpenTagAttributesToDebugStream(stream);
    }
}

}}}
