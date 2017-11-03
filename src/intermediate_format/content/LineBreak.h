/**
 * intermediate_format/content/LineBreak.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): bf20715623b3ced51517ed1937e6bfff71755f348476e410748278935244a14d
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINEBREAK_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINEBREAK_H

#include "intermediate_format/content/RawMessageContentItem.h"

#include <QDataStream>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct LineBreak : RawMessageContentItem {
public:
    LineBreak();

    virtual RawMessageContentItemSubType subType() const;

    static CEDE(LineBreak) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IApparentSubjectVisitor& visitor);

    virtual void serializeToStreamImpl(QDataStream& mut_stream) const;

    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINEBREAK_H
