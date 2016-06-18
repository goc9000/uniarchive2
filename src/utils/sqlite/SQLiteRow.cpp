/**
 * utils/sqlite/SQLiteRow.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "utils/sqlite/SQLiteRow.h"
#include "utils/sqlite/SQLiteStmt.h"

#include "utils/text/decoding.h"
#include "utils/qt/shortcuts.h"
#include "utils/language/invariant.h"

namespace uniarchive2 { namespace utils { namespace sqlite {

using namespace uniarchive2::utils::text;

static QString sqlite_type_name(int type);


sqlite3_value* SQLiteRow::rawColumn(unsigned int column_index) const {
    columnIndexSanityCheck(column_index);

    return sqlite3_column_value(stmt->handle, column_index);
}

int SQLiteRow::rawColumnType(unsigned int column_index) const {
    columnIndexSanityCheck(column_index);

    return sqlite3_column_type(stmt->handle, column_index);
}

int SQLiteRow::expectRawColumnType(unsigned int column_index, int expected_type, bool null_allowed) const {
    int type = rawColumnType(column_index);

    invariant(
        (type == expected_type) || ((type == SQLITE_NULL) && null_allowed),
        "Unexpected SQLite column type (%s instead of %s)",
        QP(sqlite_type_name(type)), QP(sqlite_type_name(expected_type))
    );

    return type;
}

int64_t SQLiteRow::int64Column(unsigned int column_index) const {
    expectRawColumnType(column_index, SQLITE_INTEGER);

    return sqlite3_column_int64(stmt->handle, column_index);
}

QString SQLiteRow::utf8TextColumn(unsigned int column_index) const {
    expectRawColumnType(column_index, SQLITE_TEXT);

    char const *text_data = (char const *) sqlite3_column_text(stmt->handle, column_index);
    if (!text_data) {
        return QString();
    }

    return decode_utf8(QByteArray::fromRawData(text_data, sqlite3_column_bytes(stmt->handle, column_index)));
}

QByteArray SQLiteRow::blobColumn(unsigned int column_index) const {
    expectRawColumnType(column_index, SQLITE_BLOB);

    char const *blob_data = (char const *) sqlite3_column_blob(stmt->handle, column_index);
    if (!blob_data) {
        return QByteArray();
    }

    return QByteArray::fromRawData(blob_data, sqlite3_column_bytes(stmt->handle, column_index));
}

void SQLiteRow::columnIndexSanityCheck(unsigned int column_index) const {
    invariant(
        column_index < numColumns,
        "SQLite column index out of range (%d vs. %d columns)", column_index, numColumns
    );
}

static QString sqlite_type_name(int type) {
    switch (type) {
        case SQLITE_NULL:
            return "NULL";
        case SQLITE_INTEGER:
            return "INTEGER";
        case SQLITE_FLOAT:
            return "FLOAT";
        case SQLITE_TEXT:
            return "TEXT";
        case SQLITE_BLOB:
            return "BLOB";
        default:
            invariant_violation("Unsupported SQLite type: %d", type);
    }
}

}}}