/**
 * intermediate_format/RawConversationCollection.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_RAWCONVERSATIONCOLLECTION_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_RAWCONVERSATIONCOLLECTION_H

#include "intermediate_format/RawConversation.h"
#include "utils/language/shortcuts.h"

#include <vector>

#include <QDebug>

namespace uniarchive2 { namespace intermediate_format {

using namespace std;

class RawConversationCollection {
public:
    vector<RawConversation> conversations;

    void take(RawConversation&& conversation);
    void take(vector<RawConversation>&& conversations);
    void take(RawConversationCollection&& collection);

    vector<RawConversation>::const_iterator begin() const;
    vector<RawConversation>::const_iterator end() const;

    void writeToDebugStream(QDebug stream, bool summarize_conversations = true) const;

    static RawConversationCollection from(RawConversation&& conversation);
    static RawConversationCollection from(vector<RawConversation>&& conversations);
};

QDebug operator<< (QDebug stream, IMM(RawConversationCollection) conversations);

}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_RAWCONVERSATIONCOLLECTION_H
