/**
 * intermediate_format/content/LineBreakTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINEBREAKTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINEBREAKTAG_H

#include "intermediate_format/content/Markup.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct LineBreakTag : Markup {
public:
    LineBreakTag();

protected:
    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINEBREAKTAG_H
