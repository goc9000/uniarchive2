/**
 * intermediate_format/content/SelfClosingTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/SelfClosingTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

SelfClosingTag::SelfClosingTag() : AbstractTag() {
}

bool SelfClosingTag::isClosed() const {
    return false;
}

void SelfClosingTag::writeAttributesToDebugStream(QDebug UNUSED stream) const {
}

}}}
