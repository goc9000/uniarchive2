/**
 * utils/html/parse_html_tag_lenient.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QRegularExpression>

#include "utils/language/invariant.h"
#include "utils/html/parse_html_tag_lenient.h"

namespace uniarchive2 { namespace utils { namespace html {

bool parse_html_tag_lenient(
    IMM(QString) tag_text,
    QString& out_tag_name,
    bool& out_open,
    bool& out_closed,
    QMap<QString, QString>& out_attributes
) {
#define TAG_NAME "[a-z._-][a-z0-9._-]*"
#define VALUE_PATTERN "((?<dblq_value>\"[^\"]*\")|(?<quot_value>'[^']*')|(?<raw_value>(.(?! |/>|>))*))"
#define ATTR_PATTERN "\\s+(?<attr_name>" TAG_NAME ")(?<has_value>=" VALUE_PATTERN ")?"
    static QRegularExpression tag_pattern(
        "^\\s*<"\
        "(?<closed1>/)?"\
        "(?<tag_name>" TAG_NAME ")"\
        "(?<attributes>(" ATTR_PATTERN ")*)\\s*"\
        "(?<closed2>/)?"\
        ">\\s*$",
        QRegularExpression::CaseInsensitiveOption
    );
    static QRegularExpression attr_pattern(ATTR_PATTERN);

    auto match = tag_pattern.match(tag_text);
    if (!match.hasMatch()) {
        return false;
    }

    out_tag_name = match.captured("tag_name");
    out_open = !match.capturedLength("closed1");
    out_closed = (match.capturedLength("closed1") > 0) || (match.capturedLength("closed2") > 0);

    out_attributes = QMap<QString, QString>();

    auto iter_matches = attr_pattern.globalMatch(match.captured("attributes"));
    while (iter_matches.hasNext()) {
        auto attr_match = iter_matches.next();

        QString key = attr_match.captured("attr_name");
        QString value = "";

        if (attr_match.capturedLength("has_value")) {
            if (attr_match.capturedLength("dblq_value")) {
                value = attr_match.captured("dblq_value");
                value = value.mid(1, value.length() - 2);
            } else if (attr_match.capturedLength("quot_value")) {
                value = attr_match.captured("quot_value");
                value = value.mid(1, value.length() - 2);
            } else {
                value = attr_match.captured("raw_value");
            }

            invariant(!value.contains("&"), "Decoding HTML entities in values is not yet supported");
        }

        out_attributes.insert(key, value);
    }

    return true;
#undef ATTR_PATTERN
#undef VALUE_PATTERN
#undef TAG_NAME
}

}}}
