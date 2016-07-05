/**
 * intermediate_format/content/formatting/FontTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/FontTag.h"
#include "utils/qt/shortcuts.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace content {

FontTag::FontTag(bool closed) : closed(closed) {
}

void FontTag::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);

    stream.nospace() << "[" << (closed ? "/" : "") << "Font";

    int any = 0;
    if (faces.length() > 0) {
        stream << (any++ ? ":" : ",") << QP(faces[0]);
        if (faces.length() > 1) {
            stream << "+";
        }
    }
    if (!size.isEmpty()) {
        stream << (any++ ? ":" : ",") << QP(size);
    }
    if (color) {
        stream << (any++ ? ":" : ",") << *color;
    }
    if (!css.isEmpty()) {
        stream << (any++ ? ":" : ",") << "CSS";
    }
    stream << "]";
}

}}}
