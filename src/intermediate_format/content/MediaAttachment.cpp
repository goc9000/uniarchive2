/**
 * intermediate_format/content/MediaAttachment.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/content/MediaAttachment.h"
#include "utils/language/invariant.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

MediaAttachment::MediaAttachment()  {
}

void MediaAttachment::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);

    stream.nospace() << "[Media";
    if (!filename.isEmpty()) {
        stream << " filename=" << filename;
    }
    if (error != MediaError::NO_ERROR) {
        stream << " error=";
        switch (error) {
            case MediaError::NO_ERROR:
                never_reached();
            case MediaError::OMITTED:
                stream << "OMITTED";
        }
    }
    stream << "]";
}

}}}
