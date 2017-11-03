/**
 * intermediate_format/content/formatting/ANSIColorTag.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 5bbb0ae2b8fb4f694f0fbb7eadf8244165fcea726ac64dd62a4eccec18bd992f
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

#include <QDataStream>

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::graphics;

struct ANSIColorTag : SymmetricTag {
public:
    ANSIColor color;

    ANSIColorTag(bool open, ANSIColor color);

    virtual RawMessageContentItemSubType subType() const;

    virtual QString tagName() const;

    static CEDE(ANSIColorTag) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IApparentSubjectVisitor& visitor);

    virtual void serializeAttributesToStream(QDataStream& mut_stream) const;

    virtual void writeAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ANSICOLORTAG_H
