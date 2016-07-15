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
#include "utils/qt/shortcuts.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

AdiumArchiveFileProvenance::AdiumArchiveFileProvenance(
    IMM(QString) full_filename,
    IMM(ApparentTime) last_modified_time
) : ArchiveFileProvenance(ArchiveFormat::ADIUM, full_filename, last_modified_time) {
}

CEDE(Provenance) AdiumArchiveFileProvenance::clone() const {
    unique_ptr<Provenance> provenance = make_unique<AdiumArchiveFileProvenance>(fullFilename, lastModifiedTime);

    provenance->as<AdiumArchiveFileProvenance>()->adiumVersion = adiumVersion;
    provenance->as<AdiumArchiveFileProvenance>()->adiumBuildID = adiumBuildID;

    return provenance;
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
