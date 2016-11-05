/**
 * intermediate_format/content/RawMessageContentItem.h
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
#include "utils/language/shortcuts.h"
#include "utils/polymorphics/StandardPolymorphic.h"

#include <QDebug>

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::utils::polymorphics;

struct RawMessageContentItem : StandardPolymorphic<RawMessageContentItemSubType> {
public:
    void writeToDebugStream(QDebug stream) const;

protected:
    virtual void serializeToStreamImpl(QDataStream& mut_stream) const;

    virtual void writeToDebugStreamImpl(QDebug stream) const = 0;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_RAWMESSAGECONTENTITEM_H
