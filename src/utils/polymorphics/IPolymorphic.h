/**
 * utils/polymorphics/IPolymorphic.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_POLYMORPHICS_IPOLYMORPHIC_H
#define UNIARCHIVE2_UTILS_POLYMORPHICS_IPOLYMORPHIC_H

namespace uniarchive2 { namespace utils { namespace polymorphics {

template<typename SubTypeEnumT>
class IPolymorphic {
public:
    virtual SubTypeEnumT subType() const = 0;

    template<typename T>
    bool is() {
        return dynamic_cast<T*>(this) != nullptr;
    }

    template<typename T>
    T* as() {
        T* converted = dynamic_cast<T*>(this);
        invariant(converted, "Not an object of the specified type");
        return converted;
    }
};

}}}

#endif //UNIARCHIVE2_UTILS_POLYMORPHICS_IPOLYMORPHIC_H
