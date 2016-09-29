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

#include <QDataStream>

template<typename T>
QDataStream& operator<< (QDataStream& mut_stream, IMM(std::vector<T>) items) {
    mut_stream << (quint32)items.size();
    for (IMM(auto) item : items) {
        mut_stream << item;
    }
    return mut_stream;
}

#endif //UNIARCHIVE2_UTILS_SERIALIZATION_SERIALIZATION_HELPERS_H
