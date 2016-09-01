/**
 * intermediate_format/content/RawMessageContentItem.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/RawMessageContentItem.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace content {

void RawMessageContentItem::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace();

    writeToDebugStreamImpl(stream);
}

QDebug operator<< (QDebug stream, CPTR(RawMessageContentItem) item) {
    item->writeToDebugStream(stream);
    return stream;
}

}}}
