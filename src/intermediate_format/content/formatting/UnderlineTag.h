/**
 * intermediate_format/content/formatting/UnderlineTag.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 65af9b3307e19c9ce17a1e0044762ac84d8525eb3243eb08cbf690e1ca403894
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_UNDERLINETAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_UNDERLINETAG_H

#include "intermediate_format/content/StandardTag.h"

#include <QDataStream>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct UnderlineTag : StandardTag {
public:
    UnderlineTag(bool open);

    virtual RawMessageContentItemSubType subType() const;

    virtual QString tagName() const;

    static CEDE(UnderlineTag) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IApparentSubjectVisitor& visitor);

    virtual void serializeOpenTagAttributesToStream(QDataStream& mut_stream) const;

    virtual void writeOpenTagAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_UNDERLINETAG_H
