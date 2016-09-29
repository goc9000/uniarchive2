/**
 * utils/polymorphics/StandardPolymorphic.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_POLYMORPHICS_STANDARDPOLYMORPHIC_H
#define UNIARCHIVE2_UTILS_POLYMORPHICS_STANDARDPOLYMORPHIC_H

#include "utils/polymorphics/IPolymorphic.h"
#include "utils/language/invariant.h"
#include "utils/language/shortcuts.h"

#include <QDebug>

namespace uniarchive2 { namespace utils { namespace polymorphics {

template<typename SubTypeEnumT>
class StandardPolymorphic : public IPolymorphic<SubTypeEnumT> {
public:
    virtual void writeToDebugStream(QDebug stream) const = 0;
};

template<typename T>
QDebug operator<< (QDebug stream, IMM(StandardPolymorphic<T>) object) {
    object.writeToDebugStream(stream);
    return stream;
}
template<typename T>
QDebug operator<< (QDebug stream, CPTR(StandardPolymorphic<T>) object) {
    invariant(object != nullptr, "Attempted to debug write null polymorphic");
    object->writeToDebugStream(stream);
    return stream;
}

}}}

#endif //UNIARCHIVE2_UTILS_POLYMORPHICS_STANDARDPOLYMORPHIC_H
