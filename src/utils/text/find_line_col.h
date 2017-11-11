/**
 * utils/text/find_line_col.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_TEXT_FIND_LINE_COL_H
#define UNIARCHIVE2_UTILS_TEXT_FIND_LINE_COL_H

#include "utils/language/shortcuts.h"

#include <QByteArray>

#include <utility>

namespace uniarchive2 { namespace utils { namespace text {

using namespace std;

pair<int, int> find_line_col(IMM(QByteArray) text_as_binary, int offset);

}}}

#endif //UNIARCHIVE2_UTILS_TEXT_FIND_LINE_COL_H
