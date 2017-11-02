/**
 * intermediate_format/provenance/ArchiveEntryProvenance.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_ARCHIVEENTRYPROVENANCE_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_ARCHIVEENTRYPROVENANCE_H

#include "intermediate_format/provenance/Provenance.h"
#include "intermediate_format/ApparentTime.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

class ArchiveEntryProvenance : public Provenance {
public:
    unique_ptr<Provenance> archive;
    QString entryFilename;
    ApparentTime entryLastModifiedTime;

    ArchiveEntryProvenance(
        TAKE(Provenance) archive_provenance,
        IMM(QString) entry_filename,
        IMM(ApparentTime) entry_last_modified_time
    );

    virtual ProvenanceSubType subType() const;

    virtual CEDE(Provenance) clone() const;

    static CEDE(ArchiveEntryProvenance) deserializeFromStream(QDataStream& mut_stream, bool skip_type = false);

protected:
    virtual void serializeToStreamImpl(QDataStream& mut_stream) const;
    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_ARCHIVEENTRYPROVENANCE_H
