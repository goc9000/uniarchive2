/**
 * utils/json/json_utils.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "utils/json/json_utils.h"

#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace json {

static void _common_check_code(bool condition, IMM(QString) value_name, IMM(QString) base_error_text) {
    if (condition) {
        return;
    }

    invariant_violation("%s %s", QP(value_name), QP(base_error_text));
}

QJsonObject expect_json_object(IMM(QJsonValue) json_value, IMM(QString) value_name) {
    _common_check_code(json_value.isObject(), value_name, "is absent or not an object");

    return json_value.toObject();
}

QJsonObject expect_json_object(IMM(QJsonDocument) json_doc, IMM(QString) value_name) {
    _common_check_code(json_doc.isObject(), value_name, "is absent or not an object");

    return json_doc.object();
}

QJsonArray expect_json_array(IMM(QJsonValue) json_value, IMM(QString) value_name) {
    _common_check_code(json_value.isArray(), value_name, "is absent or not an array");

    return json_value.toArray();
}

QJsonArray expect_json_array(IMM(QJsonDocument) json_doc, IMM(QString) value_name) {
    _common_check_code(json_doc.isArray(), value_name, "is absent or not an array");

    return json_doc.array();
}

QString expect_json_string(IMM(QJsonValue) json_value, IMM(QString) value_name) {
    _common_check_code(json_value.isString(), value_name, "is absent or not a string");

    return json_value.toString();
}

}}}
