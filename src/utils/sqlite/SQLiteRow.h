/**
 * utils/sqlite/SQLiteRow.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_SQLITE_SQLITEROW_H
#define UNIARCHIVE2_UTILS_SQLITE_SQLITEROW_H

#include "utils/external_libs/sqlite/sqlite3.h"
#include "utils/language/invariant.h"
#include "utils/language/shortcuts.h"

#include <QString>
#include <QByteArray>

namespace uniarchive2 { namespace utils { namespace sqlite {

using namespace std;

class SQLiteStmt;

class SQLiteRow {
public:
    SQLiteStmt* stmt;
    uint rowIndex = 0;
    uint numColumns = 0;

    SQLiteRow(SQLiteStmt* stmt) : stmt(stmt) {}

    QString columnName(uint colum_index) const;

    sqlite3_value* rawColumn(uint column_index) const;
    int rawColumnType(uint column_index) const;
    int expectRawColumnType(uint column_index, int expected_type, bool null_allowed = true) const;

    bool boolColumn(uint column_index) const;
    int64_t int64Column(uint column_index) const;
    QString utf8TextColumn(uint column_index) const;
    QByteArray blobColumn(uint column_index) const;

private:
    void columnIndexSanityCheck(uint column_index) const;
};

}}}

#endif //UNIARCHIVE2_UTILS_SQLITE_SQLITEROW_H
