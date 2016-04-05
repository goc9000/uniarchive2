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

QDebug operator<< (QDebug stream, const IntermediateFormatMessageContent& content) {
    QDebugStateSaver saver(stream);
    stream << content.temporaryRawText;
    return stream;
}

}}}
