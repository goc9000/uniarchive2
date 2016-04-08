/**
 * intermediate_format/content/LinkTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINKTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINKTAG_H

#include <QtDebug>

#include "intermediate_format/content/Markup.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct LinkTag : Markup {
public:
    bool closed;

    LinkTag(bool closed = false);
    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINKTAG_H
