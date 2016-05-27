/**
 * utils/text/split_into_lines.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/text/split_into_lines.h"

namespace uniarchive2 { namespace utils { namespace text {

QStringList split_into_lines(IMM(QString) text) {
    QREGEX(line_terminator, "(\r\n|\r|\n)");
    QStringList lines = text.split(line_terminator);
    if (lines.last() == "") {
        lines.removeLast();
    }

    return lines;
}

}}}
