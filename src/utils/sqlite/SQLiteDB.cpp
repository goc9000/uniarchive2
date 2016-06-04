/**
 * utils/sqlite/SQLiteDB.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "utils/sqlite/SQLiteDB.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace sqlite {

SQLiteDB::SQLiteDB() {
}

SQLiteDB::SQLiteDB(SQLiteDB&& move_me) {
    *this = move(move_me);
}

SQLiteDB& SQLiteDB::operator = (SQLiteDB&& move_me) {
    if (handle && (handle != move_me.handle)) {
        this->~SQLiteDB();
    }

    handle = move_me.handle;
    move_me.handle = nullptr;

    move(move_me.statements.begin(), move_me.statements.end(), std::inserter(statements, statements.begin()));

    return *this;
}

SQLiteDB::~SQLiteDB() {
    if (!handle) {
        return;
    }

    statements.clear();

    sqlite3_close_v2(handle);
    handle = nullptr;
}

IMM(SQLiteStmt) SQLiteDB::stmt(char const * const sql) {
    auto it = statements.find(sql);
    if (it != statements.end()) {
        return it->second;
    }

    sqlite3_stmt* statement_handle = nullptr;
    int status = sqlite3_prepare_v2(handle, sql, -1, &statement_handle, nullptr);
    invariant(status == SQLITE_OK,  "Could not prepare statement '%s': %s", sql, sqlite3_errmsg(handle));

    statements.emplace(sql, SQLiteStmt(statement_handle, this));
    return statements.at(sql);
}

SQLiteDB SQLiteDB::openReadOnly(IMM(QString) filename) {
    SQLiteDB db;

    int status = sqlite3_open_v2(filename.toUtf8().constData(), &db.handle, SQLITE_OPEN_READONLY, nullptr);
    invariant(
        status == SQLITE_OK,
        "Could not open Skype SQLite database '%s': %s", QP(filename), sqlite3_errmsg(db.handle)
    );

    return db;
}

}}}
