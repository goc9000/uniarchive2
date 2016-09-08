/**
 * intermediate_format/content/StandardTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_STANDARDTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_STANDARDTAG_H

#include "intermediate_format/content/AbstractTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct StandardTag : AbstractTag {
public:
    bool open;

    StandardTag(bool open);

protected:
    virtual bool isClosed() const;
    virtual void writeAttributesToDebugStream(QDebug stream) const;
    virtual void writeOpenTagAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_STANDARDTAG_H
