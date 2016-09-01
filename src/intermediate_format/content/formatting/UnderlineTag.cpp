/**
 * intermediate_format/content/formatting/UnderlineTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/UnderlineTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

UnderlineTag::UnderlineTag(bool closed) : closed(closed) {
}

void UnderlineTag::writeToDebugStreamImpl(QDebug stream) const {
    stream << "[" << (closed ? "/" : "") << "U]";
}

}}}
