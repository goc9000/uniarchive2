/**
 * intermediate_format/content/MediaAttachment.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_MEDIAATTACHMENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_MEDIAATTACHMENT_H

#include <QtDebug>
#include <QString>

#include "intermediate_format/content/RawMessageContentItem.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct MediaAttachment : RawMessageContentItem {
public:
    enum class MediaError {
        NO_ERROR = 0,
        OMITTED
    };

    QString filename;
    MediaError error = MediaError::NO_ERROR;

    MediaAttachment();
    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_MEDIAATTACHMENT_H