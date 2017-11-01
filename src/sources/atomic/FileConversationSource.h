/**
 * sources/atomic/FileConversationSource.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_SOURCES_FILECONVERSATIONSOURCE_H
#define UNIARCHIVE2_SOURCES_FILECONVERSATIONSOURCE_H

#include "sources/atomic/AtomicConversationSource.h"

#include "utils/language/shortcuts.h"

#include <QFileInfo>

namespace uniarchive2 { namespace sources {

class FileConversationSource : public AtomicConversationSource {
public:
    FileConversationSource(IMM(QString) filename);

    QString debugName() const;

    QString logicalFilename() const;
    QString baseName() const;
    QString extension() const;

    QString materializedFilename() const;

    QByteArray fullData() const;

    CEDE(Provenance) asProvenance() const;

protected:
    QString filename;  // Note, this should be used mostly for debug/display purposes. For other tasks use fileInfo.
    QFileInfo fileInfo;
};

}}

#endif //UNIARCHIVE2_SOURCES_FILECONVERSATIONSOURCE_H
