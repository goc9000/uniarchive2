/**
 * intermediate_format/content/ANSIResetTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ANSIRESETTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ANSIRESETTAG_H

#include <QtDebug>

#include "intermediate_format/content/Markup.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct ANSIResetTag : Markup {
public:
    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ANSIRESETTAG_H
