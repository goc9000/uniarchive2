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

#include "utils/language/shortcuts.h"

#include <QDataStream>

namespace uniarchive2 { namespace utils { namespace serialization {

class ISerializable {
public:
    virtual void serializeToStream(QDataStream& mut_stream) const = 0;
protected:
    ~ISerializable() {
        // Blank protected destructor for class used as a contract-forcing interface
    }
};

QDataStream& operator<< (QDataStream& mut_stream, IMM(ISerializable) object);
QDataStream& operator<< (QDataStream& mut_stream, CPTR(ISerializable) object);

}}}

#endif //UNIARCHIVE2_UTILS_SERIALIZATION_ISERIALIZABLE_H
