/**
 * intermediate_format/content/formatting/BoldTag.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 64299037fcd5f55f13ea554aab42315bc305b40a424fdf78a2fe4a0b5b5af14c
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_BOLDTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_BOLDTAG_H

#include "intermediate_format/content/StandardTag.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct BoldTag : StandardTag {
public:
    BoldTag(bool open);

protected:
    virtual QString tagName() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_BOLDTAG_H
