/**
 * intermediate_format/content/SkypeQuote.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/content/SkypeQuote.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

SkypeQuote::SkypeQuote(
    IMM(QString) conversation_id,
    IMM(QString) message_guid,
    TAKE(ApparentSubject) author,
    IMM(ApparentTime) time,
    IMM(QString) legacy_header,
    RawMessageContent&& quote_content
) : quotedMessageConversationID(conversation_id), quotedMessageGUID(message_guid),
    quotedMessageAuthor(move(author)), quotedMessageTime(time), legacyHeader(legacy_header),
    quoteContent(move(quote_content)) {
}

void SkypeQuote::writeToDebugStreamImpl(QDebug stream) const {
    stream << "[SkypeQuote";
    stream << " conversation_id=" << quotedMessageConversationID << " message_guid=" << quotedMessageGUID;
    stream << " author=" << quotedMessageAuthor.get() << " time=" << quotedMessageTime;
    stream << " legacy_header=" << legacyHeader;
    stream << " content=" << quoteContent;
    stream << "]";
}

}}}
