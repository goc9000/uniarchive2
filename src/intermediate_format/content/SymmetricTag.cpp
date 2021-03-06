/**
 * intermediate_format/content/SymmetricTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/SymmetricTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

SymmetricTag::SymmetricTag(bool open) : AbstractTag(), open(open) {
    // Nothing else to initialize
}

bool SymmetricTag::isClosed() const {
    return !open;
}

void SymmetricTag::serializeToStreamImpl(QDataStream& mut_stream) const {
    mut_stream << open;

    serializeAttributesToStream(mut_stream);
}

}}}
