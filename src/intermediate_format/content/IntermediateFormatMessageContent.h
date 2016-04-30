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

#include <vector>
#include <memory>

#include <QtDebug>

#include "intermediate_format/content/IntermediateFormatMessageContentItem.h"
#include "utils/language/shortcuts.h"

using namespace std;

namespace uniarchive2 { namespace intermediate_format { namespace content {

class IntermediateFormatMessageContent {
public:
    IntermediateFormatMessageContent();

    vector<unique_ptr<IntermediateFormatMessageContentItem>> items;
};

QDebug operator<< (QDebug stream, IMM(IntermediateFormatMessageContent) content);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_INTERMEDIATEFORMATMESSAGECONTENT_H
