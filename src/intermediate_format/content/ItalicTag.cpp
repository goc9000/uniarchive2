/**
 * intermediate_format/content/ItalicTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/content/ItalicTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

ItalicTag::ItalicTag(bool closed) : closed(closed) {
}

void ItalicTag::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);

    stream.nospace() << "[" << (closed ? "/" : "") << "I]";
}

}}}
