/**
 * intermediate_format/content/formatting/CSSStyleTag.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): e83fff14195a53ce0b749aa1bf0c0889128fc5053983fe3333fdd0759fe6b475
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_CSSSTYLETAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_CSSSTYLETAG_H

#include "intermediate_format/content/StandardTag.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <QDataStream>

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace std::experimental;

struct CSSStyleTag : StandardTag {
public:
    optional<QString> css;

    CSSStyleTag(IMM(QString) css);
    CSSStyleTag();

    virtual RawMessageContentItemSubType subType() const;

    virtual QString tagName() const;

    static CEDE(CSSStyleTag) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IMM(visit_subjects_callback_t) callback);

    virtual void serializeOpenTagAttributesToStream(QDataStream& mut_stream) const;

    virtual void writeOpenTagAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_CSSSTYLETAG_H
