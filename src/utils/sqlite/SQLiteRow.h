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
    unsigned int rowIndex = 0;
    unsigned int numColumns = 0;

    SQLiteRow(SQLiteStmt* stmt) : stmt(stmt) {}

    sqlite3_value* rawColumn(unsigned int column_index) const;
    int rawColumnType(unsigned int column_index) const;
    int expectRawColumnType(unsigned int column_index, int expected_type, bool null_allowed = true) const;

    int64_t int64Column(unsigned int column_index) const;
    QString utf8TextColumn(unsigned int column_index) const;
    QByteArray blobColumn(unsigned int column_index) const;

private:
    void columnIndexSanityCheck(unsigned int column_index) const;
};

}}}

#endif //UNIARCHIVE2_UTILS_SQLITE_SQLITEROW_H
