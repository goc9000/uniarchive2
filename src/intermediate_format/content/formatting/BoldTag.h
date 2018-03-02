/**
 * intermediate_format/content/formatting/BoldTag.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): f4c3234fa6fe342cb0efc38e63ba6034bf99cc82f2003835b9e8faf9cce704e2
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
#include "utils/language/shortcuts.h"

#include <QDataStream>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct BoldTag : StandardTag {
public:
    BoldTag(bool open);

    virtual RawMessageContentItemSubType subType() const;

    virtual QString tagName() const;

    static CEDE(BoldTag) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IMM(visit_subjects_callback_t) callback) const;
    bool alterSubjectsImpl(IMM(alter_subjects_callback_t) callback);

    virtual void serializeOpenTagAttributesToStream(QDataStream& mut_stream) const;

    virtual void writeOpenTagAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_BOLDTAG_H
