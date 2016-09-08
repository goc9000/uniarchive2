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
#include "utils/qt/debug_extras.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

FontTag::FontTag(bool open) : StandardTag(open) {
}

QString FontTag::tagName() const {
    return "Font";
}

void FontTag::writeOpenTagAttributesToDebugStream(QDebug stream) const {
    if (faces) {
        if (faces->size() == 1) {
            stream << " face=" << faces->front();
        } else {
            stream << " faces=" << *faces;
        }
    }
    if (size) {
        stream << " size=" << *size;
    }
    if (color) {
        stream << " color=" << *color;
    }
    if (css) {
        stream << " css=" << *css;
    }
}

}}}
