/**
 * intermediate_format/content/BoldTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_BOLDTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_BOLDTAG_H

#include "intermediate_format/content/Markup.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct BoldTag : Markup {
public:
    bool closed;

    BoldTag(bool closed = false);
    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_BOLDTAG_H
