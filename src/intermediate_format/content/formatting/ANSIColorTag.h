/**
 * intermediate_format/content/formatting/ANSIColorTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ANSICOLORTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ANSICOLORTAG_H

#include "graphics/ANSIColor.h"
#include "intermediate_format/content/SymmetricTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::graphics;

struct ANSIColorTag : SymmetricTag {
public:
    ANSIColor color;

    ANSIColorTag(bool open, ANSIColor color);

protected:
    virtual QString tagName() const;
    virtual void writeAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ANSICOLORTAG_H
