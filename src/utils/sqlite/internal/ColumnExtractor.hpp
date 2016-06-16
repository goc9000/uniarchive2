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
    static T execute(IMM(SQLiteRow) row_handle, unsigned int column_index);
};
template<typename T>
struct ColumnExtractor<optional<T>> {
    static optional<T> execute(IMM(SQLiteRow) row_handle, unsigned int column_index) {
        optional<T> result;

        if (row_handle.rawColumnType(column_index) != SQLITE_NULL) {
            result = ColumnExtractor<T>::execute(row_handle, column_index);
        }

        return result;
    }
};
template<>
struct ColumnExtractor<int> {
    static int execute(IMM(SQLiteRow) row_handle, unsigned int column_index) {
        int type = row_handle.rawColumnType(column_index);
        invariant(
            (type == SQLITE_INTEGER) || (type == SQLITE_NULL),
            "Unexpected SQLite column type (%d instead of INTEGER)", type
        );
        return sqlite3_value_int(row_handle.rawColumn(column_index));
    }
};
template<>
struct ColumnExtractor<QString> {
    static QString execute(IMM(SQLiteRow) row_handle, unsigned int column_index) {
        int type = row_handle.rawColumnType(column_index);
        invariant(
            (type == SQLITE_TEXT) || (type == SQLITE_NULL),
            "Unexpected SQLite column type (%d instead of TEXT)", type
        );

        sqlite3_value* value = row_handle.rawColumn(column_index);
        char const *text_data = (char const *) sqlite3_value_text(value);

        if (!text_data) {
            return QString();
        }

        return decode_utf8(QByteArray::fromRawData(text_data, sqlite3_value_bytes(value)));
    }
};
template<>
struct ColumnExtractor<QByteArray> {
    static QByteArray execute(IMM(SQLiteRow) row_handle, unsigned int column_index) {
        int type = row_handle.rawColumnType(column_index);
        invariant(
            (type == SQLITE_BLOB) || (type == SQLITE_NULL),
            "Unexpected SQLite column type (%d instead of BLOB)", type
        );

        sqlite3_value* value = row_handle.rawColumn(column_index);
        char const *blob_data = (char const *) sqlite3_value_blob(value);

        if (!blob_data) {
            return QByteArray();
        }

        return QByteArray::fromRawData(blob_data, sqlite3_value_bytes(value));
    }
};

}}}}

#endif //UNIARCHIVE2_UTILS_SQLITE_INTERNAL_COLUMNEXTRACTOR_H
