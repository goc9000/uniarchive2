/**
 * intermediate_format/content/formatting/EmphasisTag.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 36431c97ba18a25bece2ea076ec1c0be2385a77454b3a23de6d09e7e7010d241
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

#include <QDataStream>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct EmphasisTag : StandardTag {
public:
    EmphasisTag(bool open);

    virtual RawMessageContentItemSubType subType() const;

    virtual QString tagName() const;

    static CEDE(EmphasisTag) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    virtual void serializeOpenTagAttributesToStream(QDataStream& mut_stream) const;

    virtual void writeOpenTagAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_EMPHASISTAG_H
