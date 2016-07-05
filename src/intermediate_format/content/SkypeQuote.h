/**
 * intermediate_format/content/SkypeQuote.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_SKYPEQUOTE_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_SKYPEQUOTE_H

#include "intermediate_format/content/RawMessageContentItem.h"
#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/ApparentTime.h"
#include "utils/language/shortcuts.h"

#include <QtDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::intermediate_format::subjects;

struct SkypeQuote : RawMessageContentItem {
public:
    QString quotedMessageConversationID;
    QString quotedMessageGUID;
    unique_ptr<ApparentSubject> quotedMessageAuthor;
    ApparentTime quotedMessageTime;
    QString legacyHeader;

    RawMessageContent quoteContent;

    SkypeQuote(
        IMM(QString) conversation_id,
        IMM(QString) message_guid,
        TAKE(ApparentSubject) author,
        IMM(ApparentTime) time,
        IMM(QString) legacy_header,
        RawMessageContent&& quote_content
    );
    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_SKYPEQUOTE_H
