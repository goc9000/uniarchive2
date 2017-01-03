/**
 * utils/serialization/IDeserializableStatic.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_SERIALIZATION_IDESERIALIZABLESTATIC_H
#define UNIARCHIVE2_UTILS_SERIALIZATION_IDESERIALIZABLESTATIC_H

#include <QDataStream>

namespace uniarchive2 { namespace utils { namespace serialization {

class IDeserializableStatic {
    // We can't have virtual static methods, so this is just a marker class. But all IDeserializableStatic instances
    // should have this method:

    // static TypeName deserializeFromStream(QDataStream& mut_stream)
};

}}}

#endif //UNIARCHIVE2_UTILS_SERIALIZATION_IDESERIALIZABLESTATIC_H
