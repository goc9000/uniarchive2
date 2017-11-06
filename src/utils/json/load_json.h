/**
 * utils/json/load_json.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_JSON_LOAD_JSON_H
#define UNIARCHIVE2_UTILS_JSON_LOAD_JSON_H

#include "utils/language/shortcuts.h"

#include <QString>
#include <QJsonDocument>

namespace uniarchive2 { namespace utils { namespace json {

QJsonDocument load_json_file(IMM(QString) filename);

}}}

#endif //UNIARCHIVE2_UTILS_JSON_LOAD_JSON_H
