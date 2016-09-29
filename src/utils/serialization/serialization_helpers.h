/**
 * utils/serialization/serialization_helpers.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_SERIALIZATION_SERIALIZATION_HELPERS_H
#define UNIARCHIVE2_UTILS_SERIALIZATION_SERIALIZATION_HELPERS_H

#include "utils/serialization/ISerializable.h"
#include "utils/language/shortcuts.h"
#include "utils/external_libs/optional.hpp"

#include <QDataStream>

template<typename T>
QDataStream& operator<< (QDataStream& mut_stream, IMM(std::vector<T>) items) {
    mut_stream << (quint32)items.size();
    for (IMM(auto) item : items) {
        mut_stream << item;
    }
    return mut_stream;
}

template<typename T>
QDataStream& operator<< (QDataStream& mut_stream, IMM(std::unique_ptr<T>) item) {
    invariant((bool)item, "Tried to serialze null unique_ptr");

    mut_stream << *item;

    return mut_stream;
}

template<typename T>
QDataStream& operator<< (QDataStream& mut_stream, IMM(std::experimental::optional<T>) item) {
    mut_stream << (bool)item;
    if ((bool)item) {
        mut_stream << *item;
    }

    return mut_stream;
}

template<typename T>
void serialize_optional_unique_ptr(QDataStream& mut_stream, IMM(std::unique_ptr<T>) item) {
    mut_stream << (bool)item;
    if ((bool)item) {
        mut_stream << item;
    }
}

#endif //UNIARCHIVE2_UTILS_SERIALIZATION_SERIALIZATION_HELPERS_H
