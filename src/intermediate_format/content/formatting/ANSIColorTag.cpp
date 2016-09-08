/**
 * intermediate_format/content/formatting/ANSIColorTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/ANSIColorTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

ANSIColorTag::ANSIColorTag(bool open, ANSIColor color) : SymmetricTag(open), color(color) {
}

QString ANSIColorTag::tagName() const {
    return "ANSICol";
}

void ANSIColorTag::writeAttributesToDebugStream(QDebug stream) const {
    stream << " " << color;
}

}}}
