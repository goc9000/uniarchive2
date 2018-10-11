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
#include "utils/serialization/ISerializable.h"
#include "utils/serialization/IDeserializableDynamic.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/language/invariant.h"
#include "utils/language/shortcuts.h"

#include <QDebug>
#include <QDataStream>

namespace uniarchive2 { namespace utils { namespace polymorphics {

using namespace uniarchive2::utils::serialization;

template<typename SubTypeEnumT>
class StandardPolymorphic : public IPolymorphic<SubTypeEnumT>, public ISerializable, public IDeserializableDynamic {
public:
    virtual ~StandardPolymorphic() {
        // Do nothing by default
    }

    virtual void serializeToStream(QDataStream& mut_stream) const {
        mut_stream << this->subType();
        serializeToStreamImpl(mut_stream);
    }

    virtual void writeToDebugStream(QDebug stream) const = 0;

protected:
    static void maybeDeserializeType(bool skip_type, QDataStream& mut_stream, SubTypeEnumT expected_type) {
        if (skip_type) {
            return;
        }

        SubTypeEnumT actual_type = must_deserialize(mut_stream, SubTypeEnumT);

        invariant(
            actual_type == expected_type,
            "Encountered unexpected subtype when deserializing (%d instead of %d)",
            (int)actual_type, (int)expected_type
        );
    }

    virtual void serializeToStreamImpl(QDataStream &mut_stream) const = 0;
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
