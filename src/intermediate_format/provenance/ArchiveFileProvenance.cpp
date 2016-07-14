/**
 * intermediate_format/provenance/ArchiveFileProvenance.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/provenance/ArchiveFileProvenance.h"
#include "utils/language/shortcuts.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

ArchiveFileProvenance::ArchiveFileProvenance(
    ArchiveFormat format,
    IMM(QString) full_filename,
    IMM(ApparentTime) last_modified_time
) : archiveFormat(format), fullFilename(full_filename), lastModifiedTime(last_modified_time) {
}

CEDE(Provenance) ArchiveFileProvenance::clone() const {
    return make_unique<ArchiveFileProvenance>(archiveFormat, fullFilename, lastModifiedTime);
}

void ArchiveFileProvenance::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace() << "ArchiveFile(format=" << archiveFormat << ", path=" << fullFilename;
    if (lastModifiedTime.isSpecified()) {
        stream << ", last_modified=" << lastModifiedTime;
    }
    writeArchiveDetailsToDebugStream(stream);
    stream << ")";
}

void ArchiveFileProvenance::writeArchiveDetailsToDebugStream(QDebug UNUSED stream) const {
}

CEDE(ArchiveFileProvenance) ArchiveFileProvenance::fromQFileInfo(
    ArchiveFormat archive_format,
    IMM(QFileInfo) file_info
) {
    return make_unique<ArchiveFileProvenance>(
        archive_format,
        file_info.absoluteFilePath(),
        ApparentTime::fromQDateTimeUnknownReference(file_info.lastModified())
    );
}

}}}
