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

#include "utils/sqlite/internal/ColumnExtractor.hpp"

#include <functional>
#include <experimental/tuple>
#include <unordered_set>
#include <vector>

#include <QtDebug>
#include <QMap>
#include <QString>

#include "utils/external_libs/sqlite/sqlite3.h"
#include "utils/text/decoding.h"
#include "utils/language/callback_adapter.hpp"
#include "utils/language/invariant.h"
#include "utils/language/shortcuts.h"

using namespace std;

using namespace uniarchive2::utils::language;
using namespace uniarchive2::utils::text;

namespace uniarchive2 { namespace utils { namespace sqlite {

using namespace uniarchive2::utils::sqlite::internal;

class SQLiteDB;

namespace column_extraction_mechanics {

template<int N, typename THead, typename ...TTail>
struct DataTupleGenerator {
    static tuple<THead, TTail...> execute(sqlite3_stmt* stmt) {
        return tuple_cat(
            make_tuple(ColumnExtractor<THead>::execute(stmt, N)),
            DataTupleGenerator<N + 1, TTail...>::execute(stmt)
        );
    }
};
template<int N, typename THead>
struct DataTupleGenerator<N, THead> {
    static tuple<THead> execute(sqlite3_stmt* stmt) {
        return make_tuple(ColumnExtractor<THead>::execute(stmt, N));
    }
};

}

class SQLiteStmt {
    friend class SQLiteDB;

protected:
    sqlite3_stmt* handle = nullptr;
    SQLiteDB* parentDB = nullptr;
    int lastOpStatus = SQLITE_DONE;

    SQLiteStmt(sqlite3_stmt* handle, SQLiteDB* parent_db);

    void startQuery();
    bool hasRow() const;
    void nextRow();

    template<typename ...Args>
    void forEachRowImpl(function<void (Args...)> callback) {
        startQuery();
        while (hasRow()) {
            experimental::apply(callback, column_extraction_mechanics::DataTupleGenerator<0,Args...>::execute(handle));
            nextRow();
        }
    }

    template<typename T, typename ...Args>
    vector<T> mapRowsImpl(function<T (Args...)> mapper) {
        vector<T> result;

        startQuery();
        while (hasRow()) {
            result.push_back(experimental::apply(
                mapper,
                column_extraction_mechanics::DataTupleGenerator<0,Args...>::execute(handle)
            ));
            nextRow();
        }

        return result;
    }

    template<typename T, typename ...Args>
    unordered_set<T> mapRowsToSetImpl(function<T (Args...)> mapper) {
        unordered_set<T> result;

        startQuery();
        while (hasRow()) {
            result.insert(experimental::apply(
                mapper,
                column_extraction_mechanics::DataTupleGenerator<0,Args...>::execute(handle)
            ));
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
