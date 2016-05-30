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

    return *this;
}

SQLiteDB::~SQLiteDB() {
    if (!handle) {
        return;
    }

    sqlite3_close_v2(handle);
    handle = nullptr;
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
