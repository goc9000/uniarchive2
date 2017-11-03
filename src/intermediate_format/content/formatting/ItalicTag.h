/**
 * intermediate_format/content/formatting/ItalicTag.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): f123c8103ae1f05a2aa1c01f58bdaad919b283cafd8f071e60e84ebf9decf389
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ITALICTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ITALICTAG_H

#include "intermediate_format/content/StandardTag.h"

#include <QDataStream>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct ItalicTag : StandardTag {
public:
    ItalicTag(bool open);

    virtual RawMessageContentItemSubType subType() const;

    virtual QString tagName() const;

    static CEDE(ItalicTag) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IApparentSubjectVisitor& visitor);

    virtual void serializeOpenTagAttributesToStream(QDataStream& mut_stream) const;

    virtual void writeOpenTagAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ITALICTAG_H
