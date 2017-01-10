/**
 * intermediate_format/content/formatting/ANSIResetTag.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): eac01bef23ca8627e7724b96c1d47dae83754fff4f68b3fa5d036285a8874cdb
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ANSIRESETTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ANSIRESETTAG_H

#include "intermediate_format/content/SelfClosingTag.h"

#include <QDataStream>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct ANSIResetTag : SelfClosingTag {
public:
    ANSIResetTag();

    virtual RawMessageContentItemSubType subType() const;

    virtual QString tagName() const;

    static CEDE(ANSIResetTag) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    virtual void serializeAttributesToStream(QDataStream& mut_stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_ANSIRESETTAG_H
