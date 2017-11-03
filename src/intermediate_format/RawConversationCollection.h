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
#include "intermediate_format/subjects/visitor/IApparentSubjectVisitable.h"
#include "utils/language/shortcuts.h"
#include "utils/serialization/IDeserializableStatic.h"
#include "utils/serialization/ISerializable.h"

#include <vector>

#include <QDebug>

namespace uniarchive2 { namespace intermediate_format {

using namespace std;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::utils::serialization;

class RawConversationCollection : public ISerializable, public IDeserializableStatic, public IApparentSubjectVisitable {
public:
    vector<RawConversation> conversations;

    RawConversationCollection();

    void take(RawConversation&& conversation);
    void take(vector<RawConversation>&& conversations);
    void take(RawConversationCollection&& collection);

    vector<RawConversation>::const_iterator begin() const;
    vector<RawConversation>::const_iterator end() const;

    bool visitSubjects(IApparentSubjectVisitor &visitor);

    void writeToBinaryFile(IMM(QString) filename) const;

    static RawConversationCollection deserializeFromStream(QDataStream& mut_stream);
    virtual void serializeToStream(QDataStream& mut_stream) const;

    void writeToDebugStream(QDebug stream, bool summarize_conversations = true) const;

    static RawConversationCollection from(RawConversation&& conversation);
    static RawConversationCollection from(vector<RawConversation>&& conversations);

    static RawConversationCollection loadFromBinaryFile(IMM(QString) filename);

private:
    static const uint32_t BINARY_FORMAT_VERSION = 1;
};

QDebug operator<< (QDebug stream, IMM(RawConversationCollection) conversations);

}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_RAWCONVERSATIONCOLLECTION_H
