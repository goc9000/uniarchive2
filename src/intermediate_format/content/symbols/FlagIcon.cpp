/**
 * intermediate_format/content/symbols/FlagIcon.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/symbols/FlagIcon.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

FlagIcon::FlagIcon(IMM(QString) country_code, IMM(QString) text_equivalent)
    : RawMessageContentItem(), countryCode(country_code), textEquivalent(text_equivalent) {
}

void FlagIcon::writeToDebugStreamImpl(QDebug stream) const {
    stream << "[FlagIcon country=" << countryCode << " text=" << textEquivalent << "]";
}

}}}
