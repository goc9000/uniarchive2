/**
 * utils/time/parse_date_parts.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QMap>

#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/time/parse_date_parts.h"

namespace uniarchive2 { namespace utils { namespace time {

unsigned int parse_english_month_name(IMM(QString) month_name) {
    const static QMap<QString, unsigned int> MONTH_MAP = {
        { "january",   1 },
        { "february",  2 },
        { "march",     3 },
        { "april",     4 },
        { "may",       5 },
        { "june",      6 },
        { "july",      7 },
        { "august",    8 },
        { "september", 9 },
        { "october",  10 },
        { "november", 11 },
        { "december", 12 },
    };

    QString normalized_month_name = month_name.toLower();

    if (MONTH_MAP.contains(normalized_month_name)) {
        return MONTH_MAP[normalized_month_name];
    }

    invariant_violation("Not a month name in English: \"%s\"", QP(month_name));
}

int parse_timezone_offset_in_quarters(IMM(QString) offset_text) {
    QREGEX(pattern, "^(\\+|-)(\\d{1,2}):?(\\d{2})?$");

    if (offset_text == "") {
        return 0;
    }

    auto match = pattern.match(offset_text);
    invariant(match.hasMatch(), "Invalid timezone offset format: \"%s\"", QP(offset_text));

    bool positive = match.captured(1) == "+";
    int hours = match.captured(2).toInt();
    int minutes = (match.capturedLength(3) > 0) ? match.captured(3).toInt() : 0;

    invariant(hours < 14, "Timezone offset should be <14 hours (found: %s)", QP(offset_text));
    invariant(minutes % 15 == 0, "Timezone offset should be a multiple of 15 minutes");

    return (positive ? 1 : -1) * (hours * 4 + (minutes / 15));
}

}}}
