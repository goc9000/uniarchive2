/**
 * intermediate_format/content/CSSStyleTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/content/CSSStyleTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

CSSStyleTag::CSSStyleTag(const QString& css) : css(css), closed(false) {
}

CSSStyleTag::CSSStyleTag(bool closed) : closed(closed) {
}

void CSSStyleTag::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);

    stream.nospace() << "[" << (closed ? "/" : "") << "Style";
    if (!css.isEmpty()) {
        stream << ": " << qUtf8Printable(css);
    }
    stream << "]";
}

}}}