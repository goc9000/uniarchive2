/**
 * intermediate_format/provenance/ArchiveFileProvenance.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/provenance/ArchiveFileProvenance.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"
#include "utils/language/shortcuts.h"
#include "utils/qt/debug_extras.h"

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

ArchiveFileProvenance::ArchiveFileProvenance(TAKE(Provenance) base, ArchiveFormat format)
  : base(move(base)), archiveFormat(format) {
    // Nothing else to initialize
}

ProvenanceSubType ArchiveFileProvenance::subType() const {
    return ProvenanceSubType::ARCHIVE_FILE;
}

CEDE(Provenance) ArchiveFileProvenance::clone() const {
    return make_unique<ArchiveFileProvenance>(base->clone(), archiveFormat);
}

CEDE(ArchiveFileProvenance) ArchiveFileProvenance::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, ProvenanceSubType::ARCHIVE_FILE);

    return make_unique<ArchiveFileProvenance>(
        must_deserialize(mut_stream, unique_ptr<Provenance>),
        must_deserialize(mut_stream, ArchiveFormat)
    );
}

void ArchiveFileProvenance::serializeToStreamImpl(QDataStream& mut_stream) const {
    mut_stream << base << archiveFormat;
    serializeToStreamSubImpl(mut_stream);
}

void ArchiveFileProvenance::serializeToStreamSubImpl(QDataStream& UNUSED mut_stream) const {
    // No fields to serialize
}

void ArchiveFileProvenance::writeToDebugStreamImpl(QDebug stream) const {
    stream << "ArchiveFile(from=" << base << ", format=" << archiveFormat;
    writeArchiveDetailsToDebugStream(stream);
    stream << ")";
}

void ArchiveFileProvenance::writeArchiveDetailsToDebugStream(QDebug UNUSED stream) const {
    // No fields to write
}

}}}
