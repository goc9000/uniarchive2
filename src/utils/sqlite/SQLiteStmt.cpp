/**
 * utils/sqlite/SQLiteStmt.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "utils/sqlite/SQLiteStmt.h"
#include "utils/language/invariant.h"
#include "utils/sqlite/SQLiteDB.h"

namespace uniarchive2 { namespace utils { namespace sqlite {

SQLiteStmt::SQLiteStmt(sqlite3_stmt* handle, SQLiteDB* parent_db)
    : handle(handle), parentDB(parent_db), currentRow(this) {
    // Nothing else to initialize
}

SQLiteStmt::SQLiteStmt(SQLiteStmt&& move_me) : currentRow(this) {
    *this = move(move_me);
}

SQLiteStmt& SQLiteStmt::operator = (SQLiteStmt&& move_me) {
    if (handle && (handle != move_me.handle)) {
        this->~SQLiteStmt();
    }

    handle = move_me.handle;
    move_me.handle = nullptr;

    parentDB = move_me.parentDB;

    return *this;
}

SQLiteStmt::~SQLiteStmt() {
    if (!handle) {
        return;
    }

    sqlite3_finalize(handle);
    handle = nullptr;
    parentDB = nullptr;
}

void SQLiteStmt::startQuery() {
    invariant(handle, "Trying to run a dead statement!");

    sqlite3_reset(handle);

    nextRow();
    currentRow.rowIndex = 0;
    currentRow.numColumns = (uint)sqlite3_column_count(handle);
}

bool SQLiteStmt::hasRow() const {
    return lastOpStatus == SQLITE_ROW;
}

void SQLiteStmt::nextRow() {
    lastOpStatus = sqlite3_step(handle);
    currentRow.rowIndex++;
    invariant(
        (lastOpStatus == SQLITE_ROW) || (lastOpStatus == SQLITE_DONE),
        "Query error: %s", sqlite3_errmsg(parentDB->handle)
    );
}

}}}
