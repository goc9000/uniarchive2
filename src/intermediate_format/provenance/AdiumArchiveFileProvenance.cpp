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

AdiumArchiveFileProvenance::AdiumArchiveFileProvenance(TAKE(Provenance) base)
  : ArchiveFileProvenance(move(base), ArchiveFormat::ADIUM) {
    // Nothing else to initialize
}

AdiumArchiveFileProvenance::AdiumArchiveFileProvenance(
    TAKE(Provenance) base,
    IMM(QString) adium_version,
    IMM(QString) adium_build_id
) : ArchiveFileProvenance(move(base), ArchiveFormat::ADIUM), adiumVersion(adium_version), adiumBuildID(adium_build_id) {
    // Nothing else to initialize
}

ProvenanceSubType AdiumArchiveFileProvenance::subType() const {
    return ProvenanceSubType::ADIUM_ARCHIVE_FILE;
}

CEDE(Provenance) AdiumArchiveFileProvenance::clone() const {
    return make_unique<AdiumArchiveFileProvenance>(base->clone(), adiumVersion, adiumBuildID);
}

CEDE(AdiumArchiveFileProvenance) AdiumArchiveFileProvenance::deserializeFromStream(
    QDataStream& mut_stream,
    bool skip_type
) {
    maybeDeserializeType(skip_type, mut_stream, ProvenanceSubType::ADIUM_ARCHIVE_FILE);

    unique_ptr<Provenance> base_provenance = must_deserialize(mut_stream, unique_ptr<Provenance>);

    ArchiveFormat format = must_deserialize(mut_stream, ArchiveFormat);
    invariant(format == ArchiveFormat::ADIUM, "Expected archive format ADIUM for AdiumArchiveFileProvenance");

    return make_unique<AdiumArchiveFileProvenance>(
        move(base_provenance),
        must_deserialize(mut_stream, QString),
        must_deserialize(mut_stream, QString)
    );
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

}}}
