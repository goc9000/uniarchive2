/**
 * intermediate_format/RawTransferredFile.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_RAWTRANSFERREDFILE_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_RAWTRANSFERREDFILE_H

#include "intermediate_format/RawTransferredFileTypeHint.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"
#include "utils/serialization/IDeserializableStatic.h"
#include "utils/serialization/ISerializable.h"

#include <QtDebug>
#include <QString>

#include <cstdint>

namespace uniarchive2 { namespace intermediate_format {

using namespace std::experimental;
using namespace uniarchive2::utils::serialization;

struct RawTransferredFile final : public ISerializable, public IDeserializableStatic {
public:
    QString filename;
    optional<uint64_t> size;
    optional<RawTransferredFileTypeHint> typeHint;

    explicit RawTransferredFile(IMM(QString) filename);
    RawTransferredFile(IMM(QString) filename, uint64_t size);

    static RawTransferredFile deserializeFromStream(QDataStream& mut_stream);
    virtual void serializeToStream(QDataStream& mut_stream) const;
};

QDebug operator<< (QDebug stream, IMM(RawTransferredFile) file);

}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_RAWTRANSFERREDFILE_H
