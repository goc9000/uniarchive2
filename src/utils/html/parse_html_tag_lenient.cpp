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

ParsedHTMLTagInfo parse_html_tag_lenient(IMM(QString) tag_text) {
#define TAG_NAME "[a-z._-][a-z0-9._-]*"
#define VALUE_PATTERN "((?<dblq_value>\"[^\"]*\")|(?<quot_value>'[^']*')|(?<raw_value>(.(?! |/>|>))*))"
#define ATTR_PATTERN "\\s+(?<attr_name>" TAG_NAME ")(?<has_value>=" VALUE_PATTERN ")?"
    static QRegularExpression tag_pattern(
        "^<"\
        "(?<closed1>/)?"\
        "(?<tag_name>" TAG_NAME ")"\
        "(?<attributes>(" ATTR_PATTERN ")*)\\s*"\
        "(?<closed2>/)?"\
        ">$",
        QRegularExpression::CaseInsensitiveOption
    );
    static QRegularExpression attr_pattern(ATTR_PATTERN);

    ParsedHTMLTagInfo tag_info;
    tag_info.valid = false;
    tag_info.originalText = tag_text;

    auto match = tag_pattern.match(tag_text);
    if (!match.hasMatch()) {
        return tag_info;
    }

    tag_info.valid = true;
    tag_info.tagName = match.captured("tag_name");
    tag_info.open = !match.capturedLength("closed1");
    tag_info.closed = (match.capturedLength("closed1") > 0) || (match.capturedLength("closed2") > 0);

    auto iter_matches = attr_pattern.globalMatch(match.captured("attributes"));
    while (iter_matches.hasNext()) {
        auto attr_match = iter_matches.next();

        QString key = attr_match.captured("attr_name");

        if (attr_match.capturedLength("has_value")) {
            QString value = "";

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

            tag_info.attributes.insert(key, value);
        } else {
            tag_info.noValueAttributes.push_back(key);
        }
    }

    return tag_info;
#undef ATTR_PATTERN
#undef VALUE_PATTERN
#undef TAG_NAME
}

}}}
