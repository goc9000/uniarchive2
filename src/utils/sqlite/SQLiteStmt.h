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

#include "utils/sqlite/internal/DataTupleGenerator.hpp"

#include "utils/sqlite/internal/QueryConfig.h"
#include "utils/external_libs/sqlite/sqlite3.h"
#include "utils/text/decoding.h"
#include "utils/language/callback_adapter.hpp"
#include "utils/language/invariant.h"
#include "utils/language/shortcuts.h"

#include <functional>
#include <experimental/tuple>
#include <set>
#include <vector>

#include <QtDebug>
#include <QMap>
#include <QString>

namespace uniarchive2 { namespace utils { namespace sqlite {

using namespace std;
using namespace uniarchive2::utils::language;
using namespace uniarchive2::utils::sqlite::internal;
using namespace uniarchive2::utils::text;

class SQLiteDB;
class SQLiteRow;

class SQLiteStmt {
    friend class SQLiteDB;
    friend class SQLiteRow;

protected:
    sqlite3_stmt* handle = nullptr;
    SQLiteDB* parentDB = nullptr;
    int lastOpStatus = SQLITE_DONE;

    SQLiteRow currentRow;
    QueryConfig config;

    SQLiteStmt(sqlite3_stmt* handle, SQLiteDB* parent_db);

    void startQuery();
    bool hasRow() const;
    void nextRow();

    template<typename ...Args>
    void forEachRowImpl(function<void (Args...)> callback) {
        startQuery();
        while (hasRow()) {
            experimental::apply(callback, DataTupleGenerator<0,Args...>::execute(currentRow, config));
            nextRow();
        }
    }

    template<typename T, typename ...Args>
    vector<T> mapRowsImpl(function<T (Args...)> mapper) {
        vector<T> result;

        startQuery();
        while (hasRow()) {
            result.push_back(experimental::apply(mapper, DataTupleGenerator<0,Args...>::execute(currentRow, config)));
            nextRow();
        }

        return result;
    }

    template<typename T, typename ...Args>
    set<T> mapRowsToSetImpl(function<T (Args...)> mapper) {
        set<T> result;

        startQuery();
        while (hasRow()) {
            result.insert(experimental::apply(mapper, DataTupleGenerator<0,Args...>::execute(currentRow, config)));
            nextRow();
        }

        return result;
    }

public:
    SQLiteStmt(IMM(SQLiteStmt) copy_me) = delete;
    SQLiteStmt(SQLiteStmt&& move_me);
    SQLiteStmt& operator = (SQLiteStmt&& move_me);
    ~SQLiteStmt();

    template<typename F>
    void forEachRow(F&& callback) {
        forEachRowImpl(callback_adapter(callback));
    }

    template<typename F>
    auto mapRows(F&& callback) {
        return mapRowsImpl(callback_adapter(callback));
    }

    template<typename F>
    auto mapRowsToSet(F&& callback) {
        return mapRowsToSetImpl(callback_adapter(callback));
    }
};

}}}

#endif //UNIARCHIVE2_UTILS_SQLITE_SQLITESTMT_H
