/**
 * intermediate_format/provenance/AdiumArchiveFileProvenance.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_ADIUMARCHIVEFILEPROVENANCE_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_ADIUMARCHIVEFILEPROVENANCE_H

#include "intermediate_format/provenance/ArchiveFileProvenance.h"
#include "intermediate_format/ApparentTime.h"
#include "protocols/ArchiveFormat.h"
#include "utils/language/shortcuts.h"

#include <QFileInfo>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

using namespace uniarchive2::protocols;

class AdiumArchiveFileProvenance : public ArchiveFileProvenance {
public:
    QString adiumVersion;
    QString adiumBuildID;

    AdiumArchiveFileProvenance(IMM(QString) full_filename, IMM(ApparentTime) last_modified_time);

    virtual CEDE(Provenance) clone() const;

    virtual void writeArchiveDetailsToDebugStream(QDebug stream) const;

    static CEDE(AdiumArchiveFileProvenance) fromQFileInfo(IMM(QFileInfo) file_info);
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_ADIUMARCHIVEFILEPROVENANCE_H
