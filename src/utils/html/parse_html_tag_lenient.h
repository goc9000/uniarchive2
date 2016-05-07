/**
 * utils/html/parse_html_tag_lenient.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_HTML_PARSE_HTML_TAG_LENIENT_H
#define UNIARCHIVE2_UTILS_HTML_PARSE_HTML_TAG_LENIENT_H

#include <vector>

#include <QMap>
#include <QString>

#include "utils/language/shortcuts.h"

using namespace std;

namespace uniarchive2 { namespace utils { namespace html {

struct ParsedHTMLTagInfo {
    bool valid;
    QString tagName;
    bool open;
    bool closed;
    QMap<QString, QString> attributes;
    vector<QString> noValueAttributes;
    QString originalText;
};

ParsedHTMLTagInfo parse_html_tag_lenient(IMM(QString) tag_text);

}}}

#endif //UNIARCHIVE2_UTILS_HTML_PARSE_HTML_TAG_LENIENT_H
