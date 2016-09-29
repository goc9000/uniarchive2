/**
 * utils/serialization/ISerializable.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "utils/serialization/ISerializable.h"
#include "utils/language/invariant.h"

namespace uniarchive2 { namespace utils { namespace serialization {

QDataStream& operator<< (QDataStream& mut_stream, IMM(ISerializable) object) {
    object.serializeToStream(mut_stream);
    return mut_stream;
}

QDataStream& operator<< (QDataStream& mut_stream, CPTR(ISerializable) object) {
    invariant(object != nullptr, "Tried so serialize null object");

    object->serializeToStream(mut_stream);
    return mut_stream;
}

}}}
