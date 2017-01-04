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

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

ArchiveFileProvenance::ArchiveFileProvenance(
    ArchiveFormat format,
    IMM(QString) full_filename,
    IMM(ApparentTime) last_modified_time
) : archiveFormat(format), fullFilename(full_filename), lastModifiedTime(last_modified_time) {
}

ProvenanceSubType ArchiveFileProvenance::subType() const {
    return ProvenanceSubType::ARCHIVE_FILE;
}

CEDE(Provenance) ArchiveFileProvenance::clone() const {
    return make_unique<ArchiveFileProvenance>(archiveFormat, fullFilename, lastModifiedTime);
}

CEDE(ArchiveFileProvenance) ArchiveFileProvenance::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, ProvenanceSubType::ARCHIVE_FILE);

    return make_unique<ArchiveFileProvenance>(
        must_deserialize(mut_stream, ArchiveFormat),
        must_deserialize(mut_stream, QString),
        must_deserialize(mut_stream, ApparentTime)
    );
}

void ArchiveFileProvenance::serializeToStreamImpl(QDataStream& mut_stream) const {
    mut_stream << archiveFormat << fullFilename << lastModifiedTime;
    serializeToStreamSubImpl(mut_stream);
}

void ArchiveFileProvenance::serializeToStreamSubImpl(QDataStream& UNUSED mut_stream) const {
    // No fields to serialize
}

void ArchiveFileProvenance::writeToDebugStreamImpl(QDebug stream) const {
    stream << "ArchiveFile(format=" << archiveFormat << ", path=" << fullFilename;
    if (lastModifiedTime.isSpecified()) {
        stream << ", last_modified=" << lastModifiedTime;
    }
    writeArchiveDetailsToDebugStream(stream);
    stream << ")";
}

void ArchiveFileProvenance::writeArchiveDetailsToDebugStream(QDebug UNUSED stream) const {
}

CEDE(ArchiveFileProvenance) ArchiveFileProvenance::fromQFileInfo(
    ArchiveFormat archive_format,
    IMM(QFileInfo) file_info
) {
    return make_unique<ArchiveFileProvenance>(
        archive_format,
        file_info.absoluteFilePath(),
        ApparentTime::fromQDateTimeUnknownReference(file_info.lastModified())
    );
}

}}}
