/**
 * intermediate_format/content/RawMessageContent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/content/RawMessageContent.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

RawMessageContent::RawMessageContent() {
}

void RawMessageContent::addItem(TAKE(IntermediateFormatMessageContentItem) item) {
    if (item) {
        items.push_back(move(item));
    }
}

QDebug operator<< (QDebug stream, IMM(RawMessageContent) content) {
    QDebugStateSaver saver(stream);
    stream.nospace();

    bool first = true;

    for (auto& item : content.items) {
        stream << (first ? "" : " ") << item.get();
        first = false;
    }

    return stream;
}

}}}
