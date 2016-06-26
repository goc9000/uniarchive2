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
#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::protocols;
using namespace std;

class ArchiveFileProvenance : public Provenance {
public:
    ArchiveFormat archiveFormat;
    QString fullFilename;
    ApparentTime lastModifiedTime;

    ArchiveFileProvenance(ArchiveFormat format, IMM(QString) full_filename, IMM(ApparentTime) last_modified_time);

    virtual CEDE(Provenance) clone() const;

    virtual void writeToDebugStream(QDebug stream) const;

    static CEDE(ArchiveFileProvenance) fromQFileInfo(ArchiveFormat format, IMM(QFileInfo) file_info);
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_ARCHIVEFILEPROVENANCE_H
