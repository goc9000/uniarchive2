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

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace content {

FlagIcon::FlagIcon(IMM(QString) country_code, IMM(QString) text_equivalent)
    : countryCode(country_code), textEquivalent(text_equivalent) {
}

void FlagIcon::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);

    stream.nospace() << "[FlagIcon country=" << countryCode << " text=" << textEquivalent << "]";
}

}}}
