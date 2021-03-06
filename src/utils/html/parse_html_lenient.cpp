/**
 * utils/html/parse_html_lenient.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QChar>
#include <QDebugStateSaver>

#include "utils/html/entities.h"
#include "utils/html/parse_html_lenient.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace html {

ParsedHTMLTagInfo parse_html_tag_lenient(IMM(QString) tag_text) {
#define TAG_NAME "[a-z._-][a-z0-9._-]*"
#define VALUE_PATTERN \
    "(("\
        "?<dblq_value>\"[^\"]*\""\
    ")|("\
        "?<quot_value>'([^']|'(?! |/>|>))*'"\
    ")|("\
        "?<raw_value>(.(?! |/>|>))*"\
    "))"
#define ATTR_PATTERN "\\s+(?<attr_name>" TAG_NAME ")(?<has_value>=" VALUE_PATTERN ")?"
    ParsedHTMLTagInfo tag_info;
    tag_info.valid = false;
    tag_info.originalText = tag_text;

    QREGEX_MATCH_CI(
        match,
        "^<"\
        "(?<closed1>/)?"\
        "(?<tag_name>" TAG_NAME ")"\
        "(?<attributes>(" ATTR_PATTERN ")*)\\s*"\
        "(?<closed2>/)?"\
        ">$",
        tag_text
    );
    if (!match.hasMatch()) {
        return tag_info;
    }

    tag_info.valid = true;
    tag_info.tagName = match.captured("tag_name");
    tag_info.open = !match.capturedLength("closed1");
    tag_info.closed = (match.capturedLength("closed1") > 0) || (match.capturedLength("closed2") > 0);

    QREGEX_CI(attr_pattern, ATTR_PATTERN);
    auto iter_matches = attr_pattern.globalMatch(match.captured("attributes"));
    int expected_offset = 0;
    while (iter_matches.hasNext()) {
        auto attr_match = iter_matches.next();
        invariant(
            attr_match.capturedStart(0) == expected_offset,
            "Expected attr match offset to be %d, instead it is %d (text: \"%s\")",
            expected_offset, attr_match.capturedStart(0), QP(match.captured("attributes"))
        );

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

            tag_info.attributes.insert(key, decode_html_entities(value));
        } else {
            tag_info.noValueAttributes.push_back(key);
        }

        expected_offset = attr_match.capturedEnd(0);
    }

    return tag_info;
#undef ATTR_PATTERN
#undef VALUE_PATTERN
#undef TAG_NAME
}

ParsedHTMLInfo parse_html_lenient(IMM(QString) html_text) {
    ParsedHTMLInfo parsed_html;

    QString current_text = "";
    int position = 0;

    while (true) {
        int next_pos = html_text.indexOf(QChar('<'), position);
        if (next_pos < 0) {
            break;
        }
        int next_pos2 = html_text.indexOf(QChar('>'), next_pos);
        if (next_pos2 < 0) {
            break;
        }

        ParsedHTMLTagInfo tag_info = parse_html_tag_lenient(html_text.mid(next_pos, next_pos2 - next_pos + 1));
        if (!tag_info.valid) {
            current_text += html_text.mid(position, next_pos - position + 1);
            position = next_pos + 1;
            continue;
        }

        current_text += html_text.mid(position, next_pos - position);
        parsed_html.textSections.push_back(decode_html_entities(current_text));
        parsed_html.tags.push_back(tag_info);
        current_text.clear();
        position = next_pos2 + 1;
    }

    current_text += html_text.mid(position);
    parsed_html.textSections.push_back(decode_html_entities(current_text));

    return parsed_html;
}

QDebug operator<< (QDebug stream, IMM(ParsedHTMLTagInfo) tag_info) {
    QDebugStateSaver save(stream);
    stream.nospace();

    if (!tag_info.valid) {
        stream << "(INVALID TAG, text=" << tag_info.originalText << ")";
        return stream;
    }

    stream << "<";
    if (!tag_info.open && tag_info.closed) {
        stream << "/";
    }
    stream << QP(tag_info.tagName);

    for (IMM(QString) attr_name : tag_info.attributes.keys()) {
        stream << " " << QP(attr_name) << "=" << tag_info.attributes[attr_name];
    }
    for (IMM(QString) attr_name : tag_info.noValueAttributes) {
        stream << " [" << QP(attr_name) << "]";
    }

    if (tag_info.open && tag_info.closed) {
        stream << "/";
    }
    stream << ">";

    return stream;
}

QDebug operator<< (QDebug stream, IMM(ParsedHTMLInfo) parse_info) {
    QDebugStateSaver save(stream);
    stream.nospace();

    stream << "[\n";

    for (size_t i = 0; i < parse_info.tags.size(); i++) {
        stream << "    " << parse_info.textSections[i] << "\n";
        stream << "    " << parse_info.tags[i] << "\n";
    }
    stream << "    " << parse_info.textSections.back() << "\n";

    stream << "]\n";

    return stream;
}

}}}
