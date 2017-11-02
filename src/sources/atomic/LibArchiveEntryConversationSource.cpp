/**
 * sources/atomic/LibArchiveEntryConversationSource.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "sources/atomic/LibArchiveEntryConversationSource.h"

#include "intermediate_format/provenance/FileProvenance.h"
#include "intermediate_format/provenance/ArchiveEntryProvenance.h"
#include "intermediate_format/ApparentTime.h"

#include "utils/qt/shortcuts.h"

#include <QDir>
#include <QBuffer>

namespace uniarchive2 { namespace sources {

using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::provenance;


LibArchiveEntryConversationSource::LibArchiveEntryConversationSource(
    IMM(QString) archiveFilename,
    struct archive *mut_arch,
    struct archive_entry *mut_entry
) : archiveFileInfo(archiveFilename) {
    invariant(archiveFileInfo.exists(), "Archive file does not exist: %s", QP(archiveFilename));
    invariant(archive_entry_filetype(mut_entry) == AE_IFREG, "Only regular file entries can be handled");

    entryFilename = QString::fromUtf8(archive_entry_pathname_utf8(mut_entry));

    if (archive_entry_mtime_is_set(mut_entry)) {
        entryLastModified =
            ApparentTime::fromQDateTimeUnknownReference(QDateTime::fromTime_t(archive_entry_mtime(mut_entry)));
    }

    invariant(archive_entry_size_is_set(mut_entry), "Size not set for archive entry: %s", QP(entryFilename));

    int size = archive_entry_size(mut_entry);
    invariant(size >= 0, "Invalid size for archive entry: %s", QP(entryFilename));
    invariant(size <= MAX_EXTRACTED_ENTRY_SIZE, "Archive entry too large to load: %s", QP(entryFilename));

    data.resize(size);

    int bytes_read = archive_read_data(mut_arch, data.data(), size);
    invariant(bytes_read == size, "Failed to extract archive entry: %s", QP(entryFilename));
}

QString LibArchiveEntryConversationSource::debugName() const {
    return entryFilename;
}

QString LibArchiveEntryConversationSource::logicalFullFilename() const {
    QString archive_path = QDir::cleanPath(QDir::fromNativeSeparators(archiveFileInfo.absoluteFilePath()));
    QString entry_path = QDir::cleanPath(QDir::fromNativeSeparators(entryFilename));

    QString archive_basename = archive_path.section('/', -1, -1).section('.', 0, 0);
    QString first_entry_dir = entry_path.section('/', 0, 0);

    if (archive_basename == first_entry_dir) {
        archive_path = archive_path.section('/', 0, -2);
    }

    return QDir::cleanPath(archive_path + "/" + entry_path);
}

QString LibArchiveEntryConversationSource::baseName() const {
    return QDir::fromNativeSeparators(entryFilename).section('/', -1, -1).section('.', 0, -2);
}

QString LibArchiveEntryConversationSource::extension() const {
    return QDir::fromNativeSeparators(entryFilename).section('/', -1, -1).section('.', -1, -1);
}

QString LibArchiveEntryConversationSource::materializedFilename() const {
    invariant_violation("materializedFilename() not supported for archive entries!");
}

CEDE(QIODevice) LibArchiveEntryConversationSource::openDevice() const {
    unique_ptr<QIODevice> device = make_unique<QBuffer>();

    ((QBuffer*)device.get())->setData(data);

    invariant(device->open(QIODevice::ReadOnly), "Can't open QBuffer?!");

    return device;
}

QByteArray LibArchiveEntryConversationSource::fullData() const {
    return data;
}

CEDE(Provenance) LibArchiveEntryConversationSource::asProvenance() const {
    return make_unique<ArchiveEntryProvenance>(
        make_unique<FileProvenance>(
            archiveFileInfo.absoluteFilePath(),
            ApparentTime::fromQDateTimeUnknownReference(archiveFileInfo.lastModified())
        ),
        entryFilename,
        entryLastModified
    );
}

}}
