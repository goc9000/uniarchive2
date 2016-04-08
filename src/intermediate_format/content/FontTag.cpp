/**
 * intermediate_format/content/FontTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/content/FontTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

FontTag::FontTag(bool closed) : closed(closed) {
}

void FontTag::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);

    stream.nospace() << "[" << (closed ? "/" : "") << "Font";
    if (!faces.isEmpty() || size) {
        stream << ":";
    }
    if (faces.length() > 0) {
        stream << qUtf8Printable(faces[0]);
        if (faces.length() > 1) {
            stream << "+";
        }
        if (size) {
            stream << ',';
        }
    }
    if (size) {
        stream << qUtf8Printable(*size);
    }
    stream << "]";
}

}}}
