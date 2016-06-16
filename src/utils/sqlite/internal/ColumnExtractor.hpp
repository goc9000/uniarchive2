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
};
template<>
struct ColumnExtractor<int> {
    static int execute(IMM(SQLiteRow) row_handle, unsigned int column_index) {
        int64_t result = row_handle.int64Column(column_index);
        invariant(result <= INT_MAX, "Column value does not fit in int");

        return (int)result;
    }
};
template<>
struct ColumnExtractor<QString> {
    static QString execute(IMM(SQLiteRow) row_handle, unsigned int column_index) {
        return row_handle.utf8TextColumn(column_index);
    }
};
template<>
struct ColumnExtractor<QByteArray> {
    static QByteArray execute(IMM(SQLiteRow) row_handle, unsigned int column_index) {
        return row_handle.blobColumn(column_index);
    }
};

}}}}

#endif //UNIARCHIVE2_UTILS_SQLITE_INTERNAL_COLUMNEXTRACTOR_H
