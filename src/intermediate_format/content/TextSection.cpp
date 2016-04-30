/**
 * intermediate_format/content/TextSection.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/content/TextSection.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

TextSection::TextSection(IMM(QString) text) : text(text) {
}

void TextSection::writeToDebugStream(QDebug stream) const {
    stream << text;
}

}}}
