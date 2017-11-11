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

#include "utils/text/find_line_col.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

#include <QFile>
#include <QByteArray>

namespace uniarchive2 { namespace utils { namespace json {

using namespace uniarchive2::utils::text;

QJsonDocument load_json_file(IMM(QString) filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        invariant_violation("File not found: %s", QP(filename));
    }

    QByteArray data = file.readAll();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (doc.isNull()) {
        auto line_col = find_line_col(data, error.offset);
        invariant_violation(
            "Malformed JSON file: %s (at line %d, col %d)",
            QP(error.errorString()), line_col.first, line_col.second
        );
    }

    return doc;
}

}}}
