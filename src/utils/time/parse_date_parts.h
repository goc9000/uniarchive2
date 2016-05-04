/**
 * utils/time/parse_date_parts.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_TIME_PARSE_DATE_PARTS_H
#define UNIARCHIVE2_UTILS_TIME_PARSE_DATE_PARTS_H

#include <QString>

#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace time {

unsigned int parse_english_month_name(IMM(QString) month_text);
int parse_timezone_offset_in_quarters(IMM(QString) offset_text);

}}}

#endif //UNIARCHIVE2_UTILS_TIME_PARSE_DATE_PARTS_H
