/**
 * intermediate_format/provenance/ArchiveEntryProvenance.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/provenance/ArchiveEntryProvenance.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"
#include "utils/language/shortcuts.h"
#include "utils/qt/debug_extras.h"

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

ArchiveEntryProvenance::ArchiveEntryProvenance(
    TAKE(Provenance) archive_provenance,
    IMM(QString) entry_filename,
    IMM(ApparentTime) entry_last_modified_time
) : archive(move(archive_provenance)), entryFilename(entry_filename), entryLastModifiedTime(entry_last_modified_time) {
    // Nothing else to initialize
}

ProvenanceSubType ArchiveEntryProvenance::subType() const {
    return ProvenanceSubType::ARCHIVE_ENTRY;
}

CEDE(Provenance) ArchiveEntryProvenance::clone() const {
    return make_unique<ArchiveEntryProvenance>(archive->clone(), entryFilename, entryLastModifiedTime);
}

CEDE(ArchiveEntryProvenance) ArchiveEntryProvenance::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, ProvenanceSubType::ARCHIVE_ENTRY);

    return make_unique<ArchiveEntryProvenance>(
        must_deserialize(mut_stream, unique_ptr<Provenance>),
        must_deserialize(mut_stream, QString),
        must_deserialize(mut_stream, ApparentTime)
    );
}

void ArchiveEntryProvenance::serializeToStreamImpl(QDataStream& mut_stream) const {
    mut_stream << archive << entryFilename << entryLastModifiedTime;
}

void ArchiveEntryProvenance::writeToDebugStreamImpl(QDebug stream) const {
    stream << "ArchiveEntry(archive=" << archive << ", filename=" << entryFilename;

    if (entryLastModifiedTime.isSpecified()) {
        stream << ", last_modified=" << entryLastModifiedTime;
    }

    stream << ")";
}

}}}
