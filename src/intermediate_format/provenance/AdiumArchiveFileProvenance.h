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
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

class AdiumArchiveFileProvenance : public ArchiveFileProvenance {
public:
    QString adiumVersion;
    QString adiumBuildID;

    AdiumArchiveFileProvenance(TAKE(Provenance) base);
    AdiumArchiveFileProvenance(TAKE(Provenance) base, IMM(QString) adium_version, IMM(QString) adium_build_id);

    virtual ProvenanceSubType subType() const;

    virtual CEDE(Provenance) clone() const;

    static CEDE(AdiumArchiveFileProvenance) deserializeFromStream(QDataStream& mut_stream, bool skip_type = false);

protected:
    virtual void serializeToStreamSubImpl(QDataStream &mut_stream) const;

    virtual void writeArchiveDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_ADIUMARCHIVEFILEPROVENANCE_H
