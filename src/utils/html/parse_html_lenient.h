/**
 * utils/html/parse_html_lenient.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_HTML_PARSE_HTML_LENIENT_H
#define UNIARCHIVE2_UTILS_HTML_PARSE_HTML_LENIENT_H

#include "utils/language/shortcuts.h"

#include <vector>

#include <QtDebug>
#include <QMap>
#include <QString>

namespace uniarchive2 { namespace utils { namespace html {

using namespace std;

struct ParsedHTMLTagInfo {
    bool valid;
    QString tagName;
    bool open;
    bool closed;
    QMap<QString, QString> attributes;
    vector<QString> noValueAttributes;
    QString originalText;

    ParsedHTMLTagInfo(): valid(false), open(false), closed(false) {};
};
QDebug operator<< (QDebug stream, IMM(ParsedHTMLTagInfo) tag_info);

// There will always be 1 more textsection than tags. The significance is that the text is composed of the following
// elements: textSections[0] tags[0] textSections[1] tags[1] etc.. tags[N_tags-1] textSections[N_tags]
struct ParsedHTMLInfo {
    vector<QString> textSections;
    vector<ParsedHTMLTagInfo> tags;

    ParsedHTMLInfo() {};
};
QDebug operator<< (QDebug stream, IMM(ParsedHTMLInfo) parse_info);

ParsedHTMLTagInfo parse_html_tag_lenient(IMM(QString) tag_text);
ParsedHTMLInfo parse_html_lenient(IMM(QString) html_text);

}}}

#endif //UNIARCHIVE2_UTILS_HTML_PARSE_HTML_LENIENT_H
