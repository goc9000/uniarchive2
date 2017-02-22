/**
 * extraction/skype/internal/RawSkypeSharedFile.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPESHAREDFILE_H
#define UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPESHAREDFILE_H

#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <QString>

namespace uniarchive2 { namespace extraction { namespace skype { namespace internal {

using namespace std;

struct RawSkypeSharedFile {
    QString uri;
    uint64_t recordID;
    QString filename;
    QString type;

    RawSkypeSharedFile(IMM(QString) uri, uint64_t record_id, IMM(QString) filename, IMM(QString) type)
        : uri(uri), recordID(record_id), filename(filename), type(type) {
        // Nothing else to initialize
    }
};

}}}}

#endif //UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPESHAREDFILE_H
