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

#include "utils/language/polymorphic_helpers.h"
#include "utils/language/shortcuts.h"

#include <QDebug>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct RawMessageContentItem {
public:
    POLYMORPHIC_HELPERS

    void writeToDebugStream(QDebug stream) const;

protected:
    virtual void writeToDebugStreamImpl(QDebug stream) const = 0;
};

QDebug operator<< (QDebug stream, CPTR(RawMessageContentItem) content_item);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_RAWMESSAGECONTENTITEM_H
