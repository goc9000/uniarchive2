/**
 * intermediate_format/content/MediaAttachment.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/MediaAttachment.h"
#include "utils/language/invariant.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

MediaAttachment::MediaAttachment() : RawMessageContentItem() {
}

void MediaAttachment::writeToDebugStreamImpl(QDebug stream) const {
    stream << "[Media";
    if (filename) {
        stream << " filename=" << *filename;
    }
    if (error) {
        stream << " error=" << *error;
    }
    stream << "]";
}

}}}
