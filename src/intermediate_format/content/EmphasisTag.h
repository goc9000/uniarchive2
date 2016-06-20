/**
 * intermediate_format/content/EmphasisTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_EMPHASISTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_EMPHASISTAG_H

#include "intermediate_format/content/Markup.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct EmphasisTag : Markup {
public:
    bool closed;

    EmphasisTag(bool closed = false);
    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_EMPHASISTAG_H
