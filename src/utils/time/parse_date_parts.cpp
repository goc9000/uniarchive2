/**
 * utils/time/parse_date_parts.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QRegularExpression>

#include "utils/language/invariant.h"
#include "utils/time/parse_date_parts.h"

namespace uniarchive2 { namespace utils { namespace time {

unsigned int parse_english_month_name(IMM(QString) month_name) {
    static QRegularExpression pattern(
        "^((january)|(february)|(march)|(april)|(may)|(june)|(july)|(august)|(september)|(october)|"
            "(november)|(december))$"
    );

    int month = pattern.match(month_name.toLower()).lastCapturedIndex() - 1;
    invariant((month >= 1) && (month <= 12), "Not a month name in English: \"%s\"", qUtf8Printable(month_name));

    return (unsigned int)month;
}

int parse_timezone_offset_in_quarters(IMM(QString) offset_text) {
    static QRegularExpression pattern("^(\\+|-)(\\d+)(:(\\d+))?$");

    if (offset_text == "") {
        return 0;
    }

    auto match = pattern.match(offset_text);
    invariant(match.hasMatch(), "Invalid timezone offset format: \"%s\"", qUtf8Printable(offset_text));

    bool positive = match.captured(1) == "+";
    int hours = match.captured(2).toInt();
    int minutes = match.captured(4).toInt();

    invariant(hours < 14, "Timezone offset should be <14 hours");
    invariant(minutes % 15 == 0, "Timezone offset should be a multiple of 15 minutes");

    return (positive ? 1 : -1) * (hours * 4 + (minutes / 15));
}

}}}
