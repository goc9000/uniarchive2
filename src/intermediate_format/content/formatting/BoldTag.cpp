/**
 * intermediate_format/content/formatting/BoldTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/BoldTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

BoldTag::BoldTag(bool open) : StandardTag(open) {
}

QString BoldTag::tagName() const {
    return "B";
}

}}}
