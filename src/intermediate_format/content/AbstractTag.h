/**
 * intermediate_format/content/AbstractTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ABSTRACTTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ABSTRACTTAG_H

#include "intermediate_format/content/RawMessageContentItem.h"

#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct AbstractTag : RawMessageContentItem {
public:
    AbstractTag();

    virtual QString tagName() const = 0;

protected:
    virtual bool isClosed() const = 0;

    virtual void writeToDebugStreamImpl(QDebug stream) const;
    virtual void writeAttributesToDebugStream(QDebug stream) const = 0;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ABSTRACTTAG_H
