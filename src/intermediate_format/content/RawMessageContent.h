/**
 * intermediate_format/content/RawMessageContent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_RAWMESSAGECONTENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_RAWMESSAGECONTENT_H

#include <vector>
#include <memory>

#include <QtDebug>
#include <QString>

#include "intermediate_format/content/RawMessageContentItem.h"
#include "utils/language/shortcuts.h"

using namespace std;

namespace uniarchive2 { namespace intermediate_format { namespace content {

class RawMessageContent {
public:
    vector<unique_ptr<RawMessageContentItem>> items;

    RawMessageContent();
    static RawMessageContent fromPlainText(IMM(QString) text);

    void addItem(TAKE(RawMessageContentItem) item);
};

QDebug operator<< (QDebug stream, IMM(RawMessageContent) content);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_RAWMESSAGECONTENT_H
