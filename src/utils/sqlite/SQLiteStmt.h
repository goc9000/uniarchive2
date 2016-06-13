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

class SQLiteDB;

namespace column_extraction_mechanics {
    template<typename T> static T extract_data_column(sqlite3_stmt* stmt, int column_index);
    template<>
    int extract_data_column<int>(sqlite3_stmt* stmt, int column_index) {
        int type = sqlite3_column_type(stmt, column_index);
        invariant(
            (type == SQLITE_INTEGER) || (type == SQLITE_NULL),
            "Unexpected SQLite column type (%d instead of INTEGER)", type
        );
        return sqlite3_column_int(stmt, column_index);
    }
    template<>
    QString extract_data_column<QString>(sqlite3_stmt* stmt, int column_index) {
        int type = sqlite3_column_type(stmt, column_index);
        invariant(
            (type == SQLITE_TEXT) || (type == SQLITE_NULL),
            "Unexpected SQLite column type (%d instead of TEXT)", type
        );
        char const * text_data = (char const *)sqlite3_column_text(stmt, column_index);

        if (!text_data) {
            return QString();
        }

        return decode_utf8(QByteArray::fromRawData(text_data, sqlite3_column_bytes(stmt, column_index)));
    }

    template<int N, typename THead, typename ...TTail>
    struct generate_data_tuple {
        static tuple<THead, TTail...> execute(sqlite3_stmt* stmt) {
            return tuple_cat(
                make_tuple(extract_data_column<THead>(stmt, N)),
                generate_data_tuple<N + 1, TTail...>::execute(stmt)
            );
        }
    };
    template<int N, typename THead>
    struct generate_data_tuple<N, THead> {
        static tuple<THead> execute(sqlite3_stmt* stmt) {
            return make_tuple(extract_data_column<THead>(stmt, N));
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
            experimental::apply(callback, column_extraction_mechanics::generate_data_tuple<0,Args...>::execute(handle));
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
                column_extraction_mechanics::generate_data_tuple<0,Args...>::execute(handle)
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
                column_extraction_mechanics::generate_data_tuple<0,Args...>::execute(handle)
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
