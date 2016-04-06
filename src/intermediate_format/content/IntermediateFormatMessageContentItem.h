/**
 * intermediate_format/content/IntermediateFormatMessageContentItem.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_INTERMEDIATEFORMATMESSAGECONTENTITEM_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_INTERMEDIATEFORMATMESSAGECONTENTITEM_H

#include <QtDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct IntermediateFormatMessageContentItem {
public:
    virtual void writeToDebugStream(QDebug stream) const = 0;
};

QDebug operator<< (QDebug stream, IntermediateFormatMessageContentItem const * const content_item);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_INTERMEDIATEFORMATMESSAGECONTENTITEM_H
