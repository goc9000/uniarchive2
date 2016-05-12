/**
 * intermediate_format/content/RawMessageContentItem.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/content/RawMessageContentItem.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

QDebug operator<< (QDebug stream, RawMessageContentItem const * const content_item) {
    QDebugStateSaver saver(stream);
    stream.nospace();
    content_item->writeToDebugStream(stream);
    return stream;
}

}}}
