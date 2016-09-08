/**
 * intermediate_format/content/LineBreak.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINEBREAK_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINEBREAK_H

#include "intermediate_format/content/RawMessageContentItem.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct LineBreak : RawMessageContentItem {
public:
    LineBreak();

protected:
    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINEBREAK_H
