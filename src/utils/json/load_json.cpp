/**
 * utils/json/load_json.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "utils/json/load_json.h"

#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

#include <QFile>
#include <QByteArray>

namespace uniarchive2 { namespace utils { namespace json {

QJsonDocument load_json_file(IMM(QString) filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        invariant_violation("File not found: %s", QP(filename));
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    invariant(!doc.isNull(), "Malformed JSON file");

    return doc;
}

}}}
