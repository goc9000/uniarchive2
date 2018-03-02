/**
 * intermediate_format/content/formatting/UnderlineTag.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 0d1e2330de5dc88970f93e5902f93c2b228c596532ddc11c3af4a37d0cb01bcd
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
#include "utils/language/shortcuts.h"

#include <QDataStream>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct UnderlineTag : StandardTag {
public:
    UnderlineTag(bool open);

    virtual RawMessageContentItemSubType subType() const;

    virtual QString tagName() const;

    static CEDE(UnderlineTag) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IMM(visit_subjects_callback_t) callback);

    virtual void serializeOpenTagAttributesToStream(QDataStream& mut_stream) const;

    virtual void writeOpenTagAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_UNDERLINETAG_H
