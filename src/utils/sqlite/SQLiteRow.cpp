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
#include "utils/language/invariant.h"

namespace uniarchive2 { namespace utils { namespace sqlite {

sqlite3_value* SQLiteRow::rawColumn(unsigned int column_index) const {
    columnIndexSanityCheck(column_index);

    return sqlite3_column_value(stmt->handle, column_index);
}

int SQLiteRow::rawColumnType(unsigned int column_index) const {
    columnIndexSanityCheck(column_index);

    return sqlite3_column_type(stmt->handle, column_index);
}

void SQLiteRow::columnIndexSanityCheck(unsigned int column_index) const {
    invariant(
        column_index < numColumns,
        "SQLite column index out of range (%d vs. %d columns)", column_index, numColumns
    );
}

}}}
