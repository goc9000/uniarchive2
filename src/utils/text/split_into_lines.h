/**
 * utils/text/split_into_lines.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_TEXT_SPLIT_INTO_LINES_H
#define UNIARCHIVE2_UTILS_TEXT_SPLIT_INTO_LINES_H

#include <QString>
#include <QStringList>

#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace text {

QStringList split_into_lines(IMM(QString) filename);

}}}

#endif //UNIARCHIVE2_UTILS_TEXT_SPLIT_INTO_LINES_H
