/**
 * intermediate_format/content/formatting/CSSStyleTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/CSSStyleTag.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

CSSStyleTag::CSSStyleTag(IMM(QString) css) : css(css), closed(false) {
}

CSSStyleTag::CSSStyleTag(bool closed) : closed(closed) {
}

void CSSStyleTag::writeToDebugStreamImpl(QDebug stream) const {
    stream << "[" << (closed ? "/" : "") << "Style";
    if (!css.isEmpty()) {
        stream << ": " << QP(css);
    }
    stream << "]";
}

}}}
