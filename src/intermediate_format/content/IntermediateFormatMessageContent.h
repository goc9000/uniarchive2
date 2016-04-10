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

#include <memory>

#include <QtDebug>
#include <QList>

#include "intermediate_format/content/IntermediateFormatMessageContentItem.h"

using namespace std;

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct IntermediateFormatMessageContent {
public:
    IntermediateFormatMessageContent();

    QList<shared_ptr<IntermediateFormatMessageContentItem>> items;
};

QDebug operator<< (QDebug stream, const IntermediateFormatMessageContent& content);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_INTERMEDIATEFORMATMESSAGECONTENT_H
