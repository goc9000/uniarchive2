/**
 * intermediate_format/content/formatting/ItalicTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/ItalicTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

ItalicTag::ItalicTag(bool open) : StandardTag(open) {
}

QString ItalicTag::tagName() const {
    return "I";
}

}}}
