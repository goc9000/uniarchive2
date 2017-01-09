/**
 * intermediate_format/content/RawMessageContentItem.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 6526587c24b796f9608430ab98526fcbbf5de6021bb643fc996528f27b9fda86
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_RAWMESSAGECONTENTITEM_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_RAWMESSAGECONTENTITEM_H

#include "intermediate_format/content/RawMessageContentItemSubType.h"
#include "utils/polymorphics/StandardPolymorphic.h"

#include <QDataStream>
#include <QDebug>

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::utils::polymorphics;

struct RawMessageContentItem : StandardPolymorphic<RawMessageContentItemSubType> {
public:
    RawMessageContentItem();

    void writeToDebugStream(QDebug stream) const;

protected:
    virtual void serializeToStreamImpl(QDataStream& mut_stream) const = 0;

    virtual void writeToDebugStreamImpl(QDebug stream) const = 0;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_RAWMESSAGECONTENTITEM_H
