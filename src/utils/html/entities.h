/**
 * utils/html/entities.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_HTML_ENTITIES_H
#define UNIARCHIVE2_UTILS_HTML_ENTITIES_H

#include "utils/language/shortcuts.h"

#include <QChar>
#include <QString>

namespace uniarchive2 { namespace utils { namespace html {

using namespace std;

QString decode_html_entities(IMM(QString) html_text);
QChar html_entity_for_name(IMM(QString) entity_name);

}}}

#endif //UNIARCHIVE2_UTILS_HTML_ENTITIES_H
