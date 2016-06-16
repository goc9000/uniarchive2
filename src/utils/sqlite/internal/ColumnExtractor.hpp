/**
 * utils/sqlite/internal/ColumnExtractor.hpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_SQLITE_INTERNAL_COLUMNEXTRACTOR_H
#define UNIARCHIVE2_UTILS_SQLITE_INTERNAL_COLUMNEXTRACTOR_H

#include "utils/external_libs/optional.hpp"
#include "utils/external_libs/sqlite/sqlite3.h"
#include "utils/language/invariant.h"
#include "utils/language/shortcuts.h"

#include <QtDebug>
#include <QByteArray>
#include <QString>

namespace uniarchive2 { namespace utils { namespace sqlite { namespace internal {

using namespace std;
using namespace std::experimental;

template<typename T>
struct ColumnExtractor {
    static T execute(IMM(SQLiteRow) row_handle, unsigned int column_index);
    static const unsigned int advance;
};

template<>
struct ColumnExtractor<SQLiteRow const *> {
    static SQLiteRow const * execute(IMM(SQLiteRow) row_handle, unsigned int column_index) {
        return &row_handle;
    }
    static const unsigned int advance = 0;
};
template<typename T>
struct ColumnExtractor<optional<T>> {
    static optional<T> execute(IMM(SQLiteRow) row_handle, unsigned int column_index) {
        optional<T> result;

        if (row_handle.rawColumnType(column_index) != SQLITE_NULL) {
            result = ColumnExtractor<T>::execute(row_handle, column_index);
        }

        return result;
    }
    static const unsigned int advance = 1;
};
template<>
struct ColumnExtractor<int> {
    static int execute(IMM(SQLiteRow) row_handle, unsigned int column_index) {
        int64_t result = row_handle.int64Column(column_index);
        invariant(result <= INT_MAX, "Column value does not fit in int");

        return (int)result;
    }
    static const unsigned int advance = 1;
};
template<>
struct ColumnExtractor<QString> {
    static QString execute(IMM(SQLiteRow) row_handle, unsigned int column_index) {
        return row_handle.utf8TextColumn(column_index);
    }
    static const unsigned int advance = 1;
};
template<>
struct ColumnExtractor<QByteArray> {
    static QByteArray execute(IMM(SQLiteRow) row_handle, unsigned int column_index) {
        return row_handle.blobColumn(column_index);
    }
    static const unsigned int advance = 1;
};

}}}}

#endif //UNIARCHIVE2_UTILS_SQLITE_INTERNAL_COLUMNEXTRACTOR_H
