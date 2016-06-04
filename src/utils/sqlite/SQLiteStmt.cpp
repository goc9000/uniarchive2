/**
 * utils/sqlite/SQLiteStmt.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "utils/language/invariant.h"
#include "utils/sqlite/SQLiteDB.h"
#include "utils/sqlite/SQLiteStmt.h"

namespace uniarchive2 { namespace utils { namespace sqlite {

SQLiteStmt::SQLiteStmt(sqlite3_stmt* handle, SQLiteDB* parent_db)
    : handle(handle), parentDB(parent_db) {
}

SQLiteStmt::SQLiteStmt(SQLiteStmt&& move_me) {
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

}}}
