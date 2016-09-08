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

CSSStyleTag::CSSStyleTag(IMM(QString) css) : StandardTag(true), css(css) {
}

CSSStyleTag::CSSStyleTag() : StandardTag(false) {
}

QString CSSStyleTag::tagName() const {
    return "Style";
}

void CSSStyleTag::writeOpenTagAttributesToDebugStream(QDebug stream) const {
    stream << " " << *css;
}

}}}
