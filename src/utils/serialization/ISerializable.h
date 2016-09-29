/**
 * utils/serialization/ISerializable.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_SERIALIZATION_ISERIALIZABLE_H
#define UNIARCHIVE2_UTILS_SERIALIZATION_ISERIALIZABLE_H

#include <QDataStream>

#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace utils { namespace serialization {

class ISerializable {
public:
    virtual void serializeToStream(QDataStream& mut_stream) const = 0;
};

QDataStream& operator<< (QDataStream& mut_stream, IMM(ISerializable) object);
QDataStream& operator<< (QDataStream& mut_stream, CPTR(ISerializable) object);

}}}

#endif //UNIARCHIVE2_UTILS_SERIALIZATION_ISERIALIZABLE_H
