/**
 * utils/qt/debug_extras.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_QT_DEBUG_EXTRAS_H
#define UNIARCHIVE2_UTILS_QT_DEBUG_EXTRAS_H

#include "utils/language/shortcuts.h"
#include "utils/external_libs/optional.hpp"

#include <QtDebug>

#include <vector>
#include <unordered_set>
#include <set>
#include <map>

template<typename T>
QDebug operator<< (QDebug stream, IMM(std::unique_ptr<T>) value) {
    if (value) {
        stream << value.get();
    } else {
        stream << "(unset)";
    }

    return stream;
}

template<typename T>
QDebug operator<< (QDebug stream, IMM(std::experimental::optional<T>) value) {
    if (value) {
        stream << *value;
    } else {
        stream << "(empty)";
    }

    return stream;
}

template<typename T>
QDebug operator<< (QDebug stream, IMM(std::vector<T>) value) {
    QDebugStateSaver saver(stream);
    stream.nospace() << "[";

    if (!value.empty()) {
        char const * sep = " ";

        for (IMM(T) item : value) {
            stream << sep << item;
            sep = ", ";
        }
        stream << " ";
    }

    stream << "]";

    return stream;
}

template<typename T>
QDebug operator<< (QDebug stream, IMM(std::unordered_set<T>) value) {
    QDebugStateSaver saver(stream);
    stream.nospace() << "{";

    if (!value.empty()) {
        char const * sep = " ";

        for (IMM(T) item : value) {
            stream << sep << item;
            sep = ", ";
        }
        stream << " ";
    }

    stream << "}";

    return stream;
}

template<typename T>
QDebug operator<< (QDebug stream, IMM(std::set<T>) value) {
    QDebugStateSaver saver(stream);
    stream.nospace() << "{";

    if (!value.empty()) {
        char const * sep = " ";

        for (IMM(T) item : value) {
            stream << sep << item;
            sep = ", ";
        }
        stream << " ";
    }

    stream << "}";

    return stream;
}

template<typename K, typename V>
QDebug operator<< (QDebug stream, const std::map<K, V>& value) {
    QDebugStateSaver saver(stream);
    stream.nospace() << "{\n";

    for (IMM(auto) kv : value) {
        stream << "\t" << kv.first << " : " << kv.second << "\n";
    }

    stream << "}";

    return stream;
}

#endif //UNIARCHIVE2_UTILS_QT_DEBUG_EXTRAS_H
