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
#include "utils/qt/shortcuts.h"
#include "utils/serialization/serialization_helpers.h"

#include <QDebugStateSaver>
#include <QDataStream>
#include <QFile>

namespace uniarchive2 { namespace intermediate_format {

using namespace uniarchive2::utils::serialization;

RawConversationCollection RawConversationCollection::from(RawConversation&& conversation) {
    RawConversationCollection collection;

    collection.take(move(conversation));

    return collection;
}

RawConversationCollection RawConversationCollection::from(vector<RawConversation>&& conversations) {
    RawConversationCollection collection;

    collection.take(move(conversations));

    return collection;
}

void RawConversationCollection::take(RawConversation&& conversation) {
    conversations.push_back(move(conversation));
}

void RawConversationCollection::take(vector<RawConversation>&& conversations) {
    move(conversations.begin(), conversations.end(), back_inserter(this->conversations));
    conversations.clear();
}

void RawConversationCollection::take(RawConversationCollection&& collection) {
    take(move(collection.conversations));
}

vector<RawConversation>::const_iterator RawConversationCollection::begin() const {
    return conversations.cbegin();
}

vector<RawConversation>::const_iterator RawConversationCollection::end() const {
    return conversations.cend();
}

void RawConversationCollection::writeToBinaryFile(IMM(QString) filename) const {
    QFile file(filename);
    file.open(QFile::WriteOnly);
    invariant(file.isWritable(), "Could not open output file '%s'", QP(filename));

    QDataStream stream(&file);
    stream << (quint32)BINARY_FORMAT_VERSION;
    stream << this;
}

void RawConversationCollection::serializeToStream(QDataStream& mut_stream) const {
    mut_stream << conversations;
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
