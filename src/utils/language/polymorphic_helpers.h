/**
 * utils/language/polymorphic_helpers.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_LANGUAGE_POLYMORPHIC_HELPERS_H
#define UNIARCHIVE2_UTILS_LANGUAGE_POLYMORPHIC_HELPERS_H

#include "utils/language/invariant.h"

#define POLYMORPHIC_HELPERS\
    template<typename T>\
    bool is() {\
        return dynamic_cast<T*>(this) != nullptr;\
    }\
    \
    template<typename T>\
    T* as() {\
        T* converted = dynamic_cast<T*>(this);\
        invariant(converted, "Not an object of the specified type");\
        return converted;\
    }

#endif //UNIARCHIVE2_UTILS_LANGUAGE_POLYMORPHIC_HELPERS_H
