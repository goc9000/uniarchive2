/**
 * intermediate_format/RawTransferredFile.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/RawTransferredFile.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format {

RawTransferredFile::RawTransferredFile(IMM(QString) filename): filename(filename) {
}

RawTransferredFile::RawTransferredFile(IMM(QString) filename, uint64_t size): filename(filename), size(size) {
}

QDebug operator<< (QDebug stream, IMM(RawTransferredFile) file) {
    QDebugStateSaver settings(stream);
    stream.nospace() << "[File path=" << file.filename;
    if (file.size) {
        stream << " size=" << *file.size;
    }
    stream << "]";

    return stream;
}

}}
