/**
 * intermediate_format/provenance/ArchiveFileProvenance.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_ARCHIVEFILEPROVENANCE_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_ARCHIVEFILEPROVENANCE_H

#include "intermediate_format/provenance/Provenance.h"
#include "intermediate_format/ApparentTime.h"
#include "protocols/ArchiveFormat.h"
#include "utils/language/shortcuts.h"

#include <QFileInfo>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

using namespace uniarchive2::protocols;

class ArchiveFileProvenance : public Provenance {
public:
    ArchiveFormat archiveFormat;
    QString fullFilename;
    ApparentTime lastModifiedTime;

    ArchiveFileProvenance(ArchiveFormat format, IMM(QString) full_filename, IMM(ApparentTime) last_modified_time);

    virtual ProvenanceSubType subType() const;

    virtual CEDE(Provenance) clone() const;

    static CEDE(ArchiveFileProvenance) fromQFileInfo(ArchiveFormat format, IMM(QFileInfo) file_info);

protected:
    virtual void serializeToStreamImpl(QDataStream& mut_stream) const;
    virtual void serializeToStreamSubImpl(QDataStream& mut_stream) const;

    virtual void writeToDebugStreamImpl(QDebug stream) const;
    virtual void writeArchiveDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_ARCHIVEFILEPROVENANCE_H
