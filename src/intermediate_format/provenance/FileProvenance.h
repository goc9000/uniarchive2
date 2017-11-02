/**
 * intermediate_format/provenance/FileProvenance.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_FILEPROVENANCE_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_FILEPROVENANCE_H

#include "intermediate_format/provenance/Provenance.h"
#include "intermediate_format/ApparentTime.h"
#include "utils/language/shortcuts.h"

#include <QFileInfo>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

class FileProvenance : public Provenance {
public:
    QString fullFilename;
    ApparentTime lastModifiedTime;

    FileProvenance(IMM(QString) full_filename, IMM(ApparentTime) last_modified_time);

    virtual ProvenanceSubType subType() const;

    virtual CEDE(Provenance) clone() const;

    static CEDE(FileProvenance) fromQFileInfo(IMM(QFileInfo) file_info);

    static CEDE(FileProvenance) deserializeFromStream(QDataStream& mut_stream, bool skip_type = false);

protected:
    virtual void serializeToStreamImpl(QDataStream& mut_stream) const;
    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_FILEPROVENANCE_H
