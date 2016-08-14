/**
 * intermediate_format/RawConversationCollection.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/RawConversationCollection.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format {

void RawConversationCollection::take(RawConversation&& conversation) {
    conversations.push_back(move(conversation));
}

void RawConversationCollection::take(vector<RawConversation>&& conversations) {
    move(conversations.begin(), conversations.end(), back_inserter(this->conversations));
    conversations.clear();
}

vector<RawConversation>::const_iterator RawConversationCollection::begin() const {
    return conversations.cbegin();
}

vector<RawConversation>::const_iterator RawConversationCollection::end() const {
    return conversations.cend();
}

void RawConversationCollection::writeToDebugStream(QDebug stream, bool summarize_conversations) const {
    QDebugStateSaver settings(stream);
    stream.nospace();

    for (IMM(auto) convo : conversations) {
        convo.writeToDebugStream(stream, !summarize_conversations);
    }
}

QDebug operator<< (QDebug stream, IMM(RawConversationCollection) conversations) {
    conversations.writeToDebugStream(stream);
    return stream;
}

}}
