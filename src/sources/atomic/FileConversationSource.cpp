/**
 * sources/atomic/FileConversationSource.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "sources/atomic/FileConversationSource.h"

#include "intermediate_format/provenance/FileProvenance.h"
#include "intermediate_format/ApparentTime.h"

#include "utils/qt/shortcuts.h"

#include <QDir>

namespace uniarchive2 { namespace sources {

using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::provenance;

FileConversationSource::FileConversationSource(IMM(QString) filename) : filename(filename), fileInfo(filename) {
    invariant(fileInfo.exists(), "File does not exist: %s", QP(filename));
}

QString FileConversationSource::debugName() const {
    return filename;
}

QString FileConversationSource::logicalFullFilename() const {
    return QDir::fromNativeSeparators(fileInfo.absoluteFilePath());
}

QString FileConversationSource::baseName() const {
    return fileInfo.completeBaseName();
}

QString FileConversationSource::extension() const {
    return fileInfo.suffix();
}

QString FileConversationSource::materializedFilename() const {
    return fileInfo.absoluteFilePath();
}

CEDE(QIODevice) FileConversationSource::openDevice() const {
    unique_ptr<QIODevice> device = make_unique<QFile>(fileInfo.absoluteFilePath());

    if (!device->open(QIODevice::ReadOnly)) {
        qFatal("Can't open file: %s", QP(filename));
    }

    return device;
}

QByteArray FileConversationSource::fullData() const {
    // TODO: there should be a safety limit here to prevent opening excessively large files

    QFile file(fileInfo.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal("Can't open file: %s", QP(filename));
    }

    return file.readAll();
}

CEDE(Provenance) FileConversationSource::asProvenance() const {
    return make_unique<FileProvenance>(
        fileInfo.absoluteFilePath(),
        ApparentTime::fromQDateTimeUnknownReference(fileInfo.lastModified())
    );
}

}}
