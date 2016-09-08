/**
 * intermediate_format/content/formatting/ANSIResetTag.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/formatting/ANSIResetTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

ANSIResetTag::ANSIResetTag() : SelfClosingTag() {
}

QString ANSIResetTag::tagName() const {
    return "ANSIReset";
}

}}}
