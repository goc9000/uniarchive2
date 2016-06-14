/**
 * utils/sqlite/internal/ColumnExtractor.hpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_SQLITE_INTERNAL_COLUMNEXTRACTOR_H
#define UNIARCHIVE2_UTILS_SQLITE_INTERNAL_COLUMNEXTRACTOR_H

#include "utils/external_libs/optional.hpp"
#include "utils/external_libs/sqlite/sqlite3.h"
#include "utils/text/decoding.h"
#include "utils/language/invariant.h"
#include "utils/language/shortcuts.h"

#include <QtDebug>
#include <QByteArray>
#include <QString>

namespace uniarchive2 { namespace utils { namespace sqlite { namespace internal {

using namespace std;
using namespace std::experimental;
using namespace uniarchive2::utils::text;

template<typename T>
struct ColumnExtractor {
    static T execute(sqlite3_stmt* stmt, int column_index);
};
template<typename T>
struct ColumnExtractor<optional<T>> {
    static optional<T> execute(sqlite3_stmt *stmt, int column_index) {
        optional<T> result;

        if (sqlite3_column_type(stmt, column_index) != SQLITE_NULL) {
            result = ColumnExtractor<T>::execute(stmt, column_index);
        }

        return result;
    }
};
template<>
struct ColumnExtractor<int> {
    static int execute(sqlite3_stmt *stmt, int column_index) {
        int type = sqlite3_column_type(stmt, column_index);
        invariant(
            (type == SQLITE_INTEGER) || (type == SQLITE_NULL),
            "Unexpected SQLite column type (%d instead of INTEGER)", type
        );
        return sqlite3_column_int(stmt, column_index);
    }
};
template<>
struct ColumnExtractor<QString> {
    static QString execute(sqlite3_stmt *stmt, int column_index) {
        int type = sqlite3_column_type(stmt, column_index);
        invariant(
            (type == SQLITE_TEXT) || (type == SQLITE_NULL),
            "Unexpected SQLite column type (%d instead of TEXT)", type
        );
        char const *text_data = (char const *) sqlite3_column_text(stmt, column_index);

        if (!text_data) {
            return QString();
        }

        return decode_utf8(QByteArray::fromRawData(text_data, sqlite3_column_bytes(stmt, column_index)));
    }
};
template<>
struct ColumnExtractor<QByteArray> {
    static QByteArray execute(sqlite3_stmt *stmt, int column_index) {
        int type = sqlite3_column_type(stmt, column_index);
        invariant(
            (type == SQLITE_BLOB) || (type == SQLITE_NULL),
            "Unexpected SQLite column type (%d instead of BLOB)", type
        );
        char const *blob_data = (char const *) sqlite3_column_blob(stmt, column_index);

        if (!blob_data) {
            return QByteArray();
        }

        return QByteArray::fromRawData(blob_data, sqlite3_column_bytes(stmt, column_index));
    }
};

}}}}

#endif //UNIARCHIVE2_UTILS_SQLITE_INTERNAL_COLUMNEXTRACTOR_H
