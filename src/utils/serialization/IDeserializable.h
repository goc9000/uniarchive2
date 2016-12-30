/**
 * utils/serialization/IDeserializable.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_SERIALIZATION_IDESERIALIZABLE_H
#define UNIARCHIVE2_UTILS_SERIALIZATION_IDESERIALIZABLE_H

#include "utils/language/shortcuts.h"

#include <QDataStream>

namespace uniarchive2 { namespace utils { namespace serialization {

class IDeserializable {
    // We can't have virtual constructors, so this is just a marker class. But all IDeserializable instances should
    // have a constructor that takes a QDataStream& parameter.
};

}}}

#endif //UNIARCHIVE2_UTILS_SERIALIZATION_IDESERIALIZABLE_H
