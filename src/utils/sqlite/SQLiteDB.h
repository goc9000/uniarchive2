/**
 * utils/sqlite/SQLiteDB.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_SQLITE_SQLITEDB_H
#define UNIARCHIVE2_UTILS_SQLITE_SQLITEDB_H

#include <QString>

#include "utils/external_libs/sqlite/sqlite3.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace sqlite {

class SQLiteDB {
protected:
    sqlite3* handle = nullptr;

    SQLiteDB();

public:
    SQLiteDB(IMM(SQLiteDB) copy_me) = delete;
    SQLiteDB(SQLiteDB&& move_me);
    SQLiteDB& operator = (SQLiteDB&& move_me);
    ~SQLiteDB();
    static SQLiteDB openReadOnly(IMM(QString) filename);
};

}}}

#endif //UNIARCHIVE2_UTILS_SQLITE_SQLITEDB_H
