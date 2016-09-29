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
};

}}}

#endif //UNIARCHIVE2_UTILS_POLYMORPHICS_IPOLYMORPHIC_H
