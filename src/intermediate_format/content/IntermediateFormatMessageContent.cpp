/**
 * intermediate_format/content/IntermediateFormatMessageContent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/content/IntermediateFormatMessageContent.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

IntermediateFormatMessageContent::IntermediateFormatMessageContent() {
}

QDebug operator<< (QDebug stream, const IntermediateFormatMessageContent& content) {
    QDebugStateSaver saver(stream);
    bool first = true;

    for (auto& item : content.items) {
        stream << (first ? "" : " ") << item.get();
        first = false;
    }

    return stream;
}

}}}
