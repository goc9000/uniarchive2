/**
 * utils/serialization/deserialization_helpers.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_SERIALIZATION_DESERIALIZATION_HELPERS_H
#define UNIARCHIVE2_UTILS_SERIALIZATION_DESERIALIZATION_HELPERS_H

#include "utils/language/shortcuts.h"
#include "utils/language/template_utils.hpp"
#include "utils/external_libs/optional.hpp"
#include "utils/serialization/IDeserializable.h"

#include <QDataStream>

namespace uniarchive2 { namespace utils { namespace serialization {

using namespace uniarchive2::utils::language;

static std::string extract_type_name(char const * const type_name, unsigned int type_depth) {
    int start = 0;
    int end = strlen(type_name) - 1;

    for (unsigned int curr_type_depth = 0; curr_type_depth < type_depth; curr_type_depth++) {
        invariant(end > start, "Error extracting type name (%s : %d)", type_name, type_depth);

        while ((start < end) && (type_name[start] != '<')) {
            start++;
        }
        while ((start < end) && (type_name[end] != '>')) {
            end--;
        }

        invariant(
            (start < end - 1) && (type_name[start] == '<') && (type_name[end] == '>'),
            "Error extracting type name (%s : %d)", type_name, type_depth
        );

        start++;
        end--;
    }

    return std::string(type_name, start, end - start + 1);
}

#define must_deserialize(stream, type) _must_deserialize_impl<type>(stream, #type, 0)

template<typename T>
typename std::enable_if<is_base_of<IDeserializable, T>::value, T>::type
_must_deserialize_impl(
    QDataStream& mut_stream,
    char const * const UNUSED type_name,
    unsigned int UNUSED type_depth
) {
    return T(mut_stream);
}

template<typename T>
typename std::enable_if<!is_vector_type<T>::value && !is_base_of<IDeserializable, T>::value, T>::type
_must_deserialize_impl(QDataStream& mut_stream, char const * const type_name, unsigned int type_depth) {
    T item;

    mut_stream >> item;

    if (mut_stream.status() != QDataStream::Ok) {
        invariant_violation("Failed to deserialize type: %s", extract_type_name(type_name, type_depth).c_str());
    }

    return item;
}

template<typename T>
typename std::enable_if<is_vector_type<T>::value, T>::type
_must_deserialize_impl(QDataStream& mut_stream, char const * const type_name, unsigned int type_depth) {
    T elements;

    uint32_t n_elements = must_deserialize(mut_stream, uint32_t);

    for (uint32_t i = 0; i < n_elements; i++) {
        elements.push_back(
            _must_deserialize_impl<typename extract_vector_type<T>::type>(mut_stream, type_name, type_depth + 1)
        );
    }

    return elements;
}

}}}

#endif //UNIARCHIVE2_UTILS_SERIALIZATION_DESERIALIZATION_HELPERS_H
