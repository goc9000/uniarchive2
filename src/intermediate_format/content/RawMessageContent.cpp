/**
 * intermediate_format/content/RawMessageContent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/content/TextSection.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace content {

RawMessageContent::RawMessageContent() {
}

RawMessageContent RawMessageContent::fromPlainText(IMM(QString) text) {
    RawMessageContent content;
    content.addItem(make_unique<TextSection>(text));

    return content;
}

void RawMessageContent::addItem(TAKE(RawMessageContentItem) item) {
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
