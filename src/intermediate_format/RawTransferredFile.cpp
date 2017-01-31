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
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format {

RawTransferredFile::RawTransferredFile(IMM(QString) filename): filename(filename) {
    // Nothing else to initialize
}

RawTransferredFile::RawTransferredFile(IMM(QString) filename, uint64_t size): filename(filename), size(size) {
    // Nothing else to initialize
}

RawTransferredFile RawTransferredFile::deserializeFromStream(QDataStream& mut_stream) {
    QString filename = must_deserialize(mut_stream, QString);
    optional<uint64_t> size = must_deserialize(mut_stream, optional<uint64_t>);

    if (size) {
        return RawTransferredFile(filename, *size);
    } else {
        return RawTransferredFile(filename);
    }
}

void RawTransferredFile::serializeToStream(QDataStream& mut_stream) const {
    mut_stream << filename << size;
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
