/**
 * intermediate_format/content/ANSIColorTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ANSICOLORTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ANSICOLORTAG_H

#include <QtDebug>

#include "graphics/ANSIColor.h"
#include "intermediate_format/content/Markup.h"

using namespace uniarchive2::graphics;

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct ANSIColorTag : Markup {
public:
    ANSIColor color;
    bool closed;

    ANSIColorTag(ANSIColor color, bool closed = false);
    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ANSICOLORTAG_H
