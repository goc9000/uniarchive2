/**
 * intermediate_format/content/formatting/EmphasisTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_EMPHASISTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_EMPHASISTAG_H

#include "intermediate_format/content/StandardTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct EmphasisTag : StandardTag {
public:
    EmphasisTag(bool open);

protected:
    virtual QString tagName() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_EMPHASISTAG_H
