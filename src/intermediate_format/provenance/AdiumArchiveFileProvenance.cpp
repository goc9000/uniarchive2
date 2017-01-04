/**
 * intermediate_format/provenance/AdiumArchiveFileProvenance.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/provenance/AdiumArchiveFileProvenance.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

AdiumArchiveFileProvenance::AdiumArchiveFileProvenance(
    IMM(QString) full_filename,
    IMM(ApparentTime) last_modified_time
) : ArchiveFileProvenance(ArchiveFormat::ADIUM, full_filename, last_modified_time) {
}

ProvenanceSubType AdiumArchiveFileProvenance::subType() const {
    return ProvenanceSubType::ADIUM_ARCHIVE_FILE;
}

CEDE(Provenance) AdiumArchiveFileProvenance::clone() const {
    unique_ptr<Provenance> provenance = make_unique<AdiumArchiveFileProvenance>(fullFilename, lastModifiedTime);

    provenance->as<AdiumArchiveFileProvenance>()->adiumVersion = adiumVersion;
    provenance->as<AdiumArchiveFileProvenance>()->adiumBuildID = adiumBuildID;

    return provenance;
}

CEDE(AdiumArchiveFileProvenance) AdiumArchiveFileProvenance::deserializeFromStream(
    QDataStream& mut_stream,
    bool skip_type
) {
    maybeDeserializeType(skip_type, mut_stream, ProvenanceSubType::ADIUM_ARCHIVE_FILE);

    ArchiveFormat format = must_deserialize(mut_stream, ArchiveFormat);

    invariant(format == ArchiveFormat::ADIUM, "Expected archive format ADIUM for AdiumArchiveFileProvenance");

    unique_ptr<AdiumArchiveFileProvenance> provenance = make_unique<AdiumArchiveFileProvenance>(
        must_deserialize(mut_stream, QString),
        must_deserialize(mut_stream, ApparentTime)
    );

    mut_stream >> provenance->adiumVersion >> provenance->adiumBuildID;

    return provenance;
}

void AdiumArchiveFileProvenance::serializeToStreamSubImpl(QDataStream& mut_stream) const {
    mut_stream << adiumVersion << adiumBuildID;
}

void AdiumArchiveFileProvenance::writeArchiveDetailsToDebugStream(QDebug stream) const {
    if (!adiumVersion.isEmpty()) {
        stream << ", adium_version=" << QP(adiumVersion);
    }
    if (!adiumBuildID.isEmpty()) {
        stream << ", adium_build_id=" << QP(adiumBuildID);
    }
}

CEDE(AdiumArchiveFileProvenance) AdiumArchiveFileProvenance::fromQFileInfo(IMM(QFileInfo) file_info) {
    return make_unique<AdiumArchiveFileProvenance>(
        file_info.absoluteFilePath(),
        ApparentTime::fromQDateTimeUnknownReference(file_info.lastModified())
    );
}

}}}
