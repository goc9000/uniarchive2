/**
 * utils/serialization/IDeserializableDynamic.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_UTILS_SERIALIZATION_IDESERIALIZABLEDYNAMIC_H
#define UNIARCHIVE2_UTILS_SERIALIZATION_IDESERIALIZABLEDYNAMIC_H

#include "utils/language/shortcuts.h"

#include <QDataStream>

namespace uniarchive2 { namespace utils { namespace serialization {

class IDeserializableDynamic {
    // We can't have virtual static methods, so this is just a marker class. But all IDeserializableDynamic instances
    // should have this method:

    // static CEDE(TypeName) deserializeFromStream(QDataStream& mut_stream)
};

}}}

#endif //UNIARCHIVE2_UTILS_SERIALIZATION_IDESERIALIZABLEDYNAMIC_H
