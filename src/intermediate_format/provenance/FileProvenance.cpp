/**
 * intermediate_format/provenance/FileProvenance.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/provenance/FileProvenance.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

FileProvenance::FileProvenance(IMM(QString) full_filename, IMM(ApparentTime) last_modified_time)
  : fullFilename(full_filename), lastModifiedTime(last_modified_time) {
    // Nothing else to initialize
}

ProvenanceSubType FileProvenance::subType() const {
    return ProvenanceSubType::FILE;
}

CEDE(Provenance) FileProvenance::clone() const {
    return make_unique<FileProvenance>(fullFilename, lastModifiedTime);
}

CEDE(FileProvenance) FileProvenance::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, ProvenanceSubType::ARCHIVE_FILE);

    return make_unique<FileProvenance>(
        must_deserialize(mut_stream, QString),
        must_deserialize(mut_stream, ApparentTime)
    );
}

void FileProvenance::serializeToStreamImpl(QDataStream& mut_stream) const {
    mut_stream << fullFilename << lastModifiedTime;
    serializeToStreamSubImpl(mut_stream);
}

void FileProvenance::serializeToStreamSubImpl(QDataStream& UNUSED mut_stream) const {
    // No fields to serialize
}

void FileProvenance::writeToDebugStreamImpl(QDebug stream) const {
    stream << "File(path=" << fullFilename;
    if (lastModifiedTime.isSpecified()) {
        stream << ", last_modified=" << lastModifiedTime;
    }
    writeArchiveDetailsToDebugStream(stream);
    stream << ")";
}

void FileProvenance::writeArchiveDetailsToDebugStream(QDebug UNUSED stream) const {
    // No fields to write
}

CEDE(FileProvenance) FileProvenance::fromQFileInfo(IMM(QFileInfo) file_info) {
    return make_unique<FileProvenance>(
        file_info.absoluteFilePath(),
        ApparentTime::fromQDateTimeUnknownReference(file_info.lastModified())
    );
}

}}}
