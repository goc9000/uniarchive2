/**
 * sources/atomic/LibArchiveEntryConversationSource.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_SOURCES_LIBARCHIVEENTRYCONVERSATIONSOURCE_H
#define UNIARCHIVE2_SOURCES_LIBARCHIVEENTRYCONVERSATIONSOURCE_H

#include "intermediate_format/ApparentTime.h"

#include "sources/atomic/AtomicConversationSource.h"

#include "utils/language/shortcuts.h"

#include <archive.h>
#include <archive_entry.h>

#include <QFileInfo>

namespace uniarchive2 { namespace sources {

using namespace uniarchive2::intermediate_format;

/**
 * This source represents an entry in an archive as read by libarchive. Unfortunately, due to the nature of the
 * library, we have to extract and read the data to memory immediately. The overhead does not seem to be too large
 * though.
 */
class LibArchiveEntryConversationSource : public AtomicConversationSource {
public:
    const int MAX_EXTRACTED_ENTRY_SIZE = (1 << 27); // 128 MB

    /**
     * Creates a source based on the current entry (mut_entry) in an archive opened for reading (mut_arch). Note that
     * data will be extracted and loaded to memory immediately. This process is not thread-safe.
     */
    LibArchiveEntryConversationSource(
        IMM(QString) archiveFilename,
        struct archive *mut_arch,
        struct archive_entry *mut_entry
    );

    QString debugName() const;

    QString logicalFullFilename() const;
    QString baseName() const;
    QString extension() const;

    QString materializedFilename() const;

    CEDE(QIODevice) openDevice() const;

    QByteArray fullData() const;

    CEDE(Provenance) asProvenance() const;

protected:
    QString entryFilename;
    ApparentTime entryLastModified;

    QFileInfo archiveFileInfo;

    QByteArray data;
};

}}

#endif //UNIARCHIVE2_SOURCES_LIBARCHIVEENTRYCONVERSATIONSOURCE_H
