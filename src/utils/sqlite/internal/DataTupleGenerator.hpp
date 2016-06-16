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

#include <utils/sqlite/SQLiteRow.h>
#include "utils/sqlite/internal/ColumnExtractor.hpp"
#include "utils/sqlite/internal/QueryConfig.h"
#include "utils/external_libs/sqlite/sqlite3.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace sqlite { namespace internal {

using namespace std;

template<unsigned int N, typename THead, typename ...TTail>
struct DataTupleGenerator {
    static tuple<THead, TTail...> execute(IMM(SQLiteRow) row_handle, IMM(QueryConfig) config) {
        return tuple_cat(
            make_tuple(ColumnExtractor<THead>::execute(row_handle, N, config)),
            DataTupleGenerator<N + 1, TTail...>::execute(row_handle, config)
        );
    }
};
template<unsigned int N, typename THead>
struct DataTupleGenerator<N, THead> {
    static tuple<THead> execute(IMM(SQLiteRow) row_handle, IMM(QueryConfig) config) {
        return make_tuple(ColumnExtractor<THead>::execute(row_handle, N, config));
    }
};

}}}}

#endif //UNIARCHIVE2_UTILS_SQLITE_INTERNAL_DATATUPLEGENERATOR_H
