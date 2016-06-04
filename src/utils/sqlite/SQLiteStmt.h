/**
 * utils/sqlite/SQLiteStmt.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_SQLITE_SQLITESTMT_H
#define UNIARCHIVE2_UTILS_SQLITE_SQLITESTMT_H

#include <QtDebug>
#include <QMap>
#include <QString>

#include "utils/external_libs/sqlite/sqlite3.h"
#include "utils/language/shortcuts.h"

using namespace std;

namespace uniarchive2 { namespace utils { namespace sqlite {

class SQLiteDB;

class SQLiteStmt {
    friend class SQLiteDB;

protected:
    sqlite3_stmt* handle = nullptr;
    SQLiteDB* parentDB = nullptr;

    SQLiteStmt(sqlite3_stmt* handle, SQLiteDB* parent_db);

public:
    SQLiteStmt(IMM(SQLiteStmt) copy_me) = delete;
    SQLiteStmt(SQLiteStmt&& move_me);
    SQLiteStmt& operator = (SQLiteStmt&& move_me);
    ~SQLiteStmt();
};

}}}

#endif //UNIARCHIVE2_UTILS_SQLITE_SQLITESTMT_H
