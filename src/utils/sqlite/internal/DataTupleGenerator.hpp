/**
 * utils/sqlite/internal/DataTupleGenerator.hpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_SQLITE_INTERNAL_DATATUPLEGENERATOR_H
#define UNIARCHIVE2_UTILS_SQLITE_INTERNAL_DATATUPLEGENERATOR_H

#include "utils/sqlite/internal/ColumnExtractor.hpp"
#include "utils/external_libs/sqlite/sqlite3.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace sqlite { namespace internal {

using namespace std;

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

}}}}

#endif //UNIARCHIVE2_UTILS_SQLITE_INTERNAL_DATATUPLEGENERATOR_H
