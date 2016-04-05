/**
 * intermediate_format/content/IntermediateFormatMessageContent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_INTERMEDIATEFORMATMESSAGECONTENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_INTERMEDIATEFORMATMESSAGECONTENT_H

#include <QtDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct IntermediateFormatMessageContent {
public:
    QString temporaryRawText; // TODO: will be refined later
};

QDebug operator<< (QDebug stream, const IntermediateFormatMessageContent& content);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_INTERMEDIATEFORMATMESSAGECONTENT_H
