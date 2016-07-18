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

#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <QtDebug>
#include <QString>

#include <cstdint>

namespace uniarchive2 { namespace intermediate_format {

using namespace std::experimental;

struct RawTransferredFile {
public:
    QString filename;
    optional<uint64_t> size;

    explicit RawTransferredFile(IMM(QString) filename);
    RawTransferredFile(IMM(QString) filename, uint64_t size);
};

QDebug operator<< (QDebug stream, IMM(RawTransferredFile) time);

}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_RAWTRANSFERREDFILE_H
