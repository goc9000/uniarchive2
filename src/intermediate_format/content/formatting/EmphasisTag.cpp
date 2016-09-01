/**
 * intermediate_format/content/formatting/EmphasisTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/EmphasisTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

EmphasisTag::EmphasisTag(bool closed) : closed(closed) {
}

void EmphasisTag::writeToDebugStreamImpl(QDebug stream) const {
    stream << "[" << (closed ? "/" : "") << "EM]";
}

}}}
