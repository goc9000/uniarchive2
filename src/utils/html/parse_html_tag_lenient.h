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

#include <QMap>
#include <QString>

#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace html {

bool parse_html_tag_lenient(
    IMM(QString) tag_text,
    QString& out_tag_name,
    bool& out_open,
    bool& out_closed,
    QMap<QString, QString>& out_attributes
);

}}}

#endif //UNIARCHIVE2_UTILS_HTML_PARSE_HTML_TAG_LENIENT_H
