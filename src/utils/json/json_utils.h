/**
 * utils/json/json_utils.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_JSON_JSON_UTILS_H
#define UNIARCHIVE2_UTILS_JSON_JSON_UTILS_H

#include "utils/language/shortcuts.h"

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

namespace uniarchive2 { namespace utils { namespace json {

QJsonObject expect_json_object(IMM(QJsonValue) json_value, IMM(QString) value_name = "JSON value");
QJsonObject expect_json_object(IMM(QJsonDocument) json_doc, IMM(QString) value_name = "JSON document root");

QJsonArray expect_json_array(IMM(QJsonValue) json_value, IMM(QString) value_name = "JSON value");
QJsonArray expect_json_array(IMM(QJsonDocument) json_doc, IMM(QString) value_name = "JSON document root");

QString expect_json_string(IMM(QJsonValue) json_value, IMM(QString) value_name = "JSON value");

}}}

#endif //UNIARCHIVE2_UTILS_JSON_JSON_UTILS_H
