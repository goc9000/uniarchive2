/**
 * utils/text/find_line_col.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace text {

pair<int, int> find_line_col(IMM(QByteArray) text_as_binary, int offset) {
    invariant(offset <= text_as_binary.length(), "Invalid offset");

    int line_no = 1;
    int search_pos = 0;

    while (search_pos < text_as_binary.length()) {
        int newline_pos = text_as_binary.indexOf('\n', search_pos);
        if ((newline_pos == -1) || (newline_pos >= offset)) {
            break;
        }

        line_no++;
        search_pos = newline_pos + 1;
    }

    return pair<int, int>(line_no, 1 + offset - search_pos);
}

}}}
