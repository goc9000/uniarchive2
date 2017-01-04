/**
 * intermediate_format/provenance/SkypeConversationProvenance.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/provenance/SkypeConversationProvenance.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

SkypeConversationProvenance::SkypeConversationProvenance(TAKE(Provenance) base, uint64_t convo_dbid, QString chatname)
    : base(move(base)), conversationDBID(convo_dbid), chatname(chatname) {
}

ProvenanceSubType SkypeConversationProvenance::subType() const {
    return ProvenanceSubType::SKYPE_CONVERSATION;
}

CEDE(Provenance) SkypeConversationProvenance::clone() const {
    unique_ptr<Provenance> dupe = make_unique<SkypeConversationProvenance>(base->clone(), conversationDBID, chatname);

    dupe->as<SkypeConversationProvenance>()->chatDBID = chatDBID;
    dupe->as<SkypeConversationProvenance>()->callDBID = callDBID;

    return dupe;
}

CEDE(SkypeConversationProvenance) SkypeConversationProvenance::deserializeFromStream(
    QDataStream& mut_stream,
    bool skip_type
) {
    maybeDeserializeType(skip_type, mut_stream, ProvenanceSubType::SKYPE_CONVERSATION);

    unique_ptr<SkypeConversationProvenance> provenance = make_unique<SkypeConversationProvenance>(
        must_deserialize(mut_stream, unique_ptr<Provenance>),
        must_deserialize(mut_stream, uint64_t),
        must_deserialize(mut_stream, QString)
    );

    mut_stream >> provenance->chatDBID >> provenance->callDBID;

    return provenance;
}

void SkypeConversationProvenance::serializeToStreamImpl(QDataStream &mut_stream) const {
    mut_stream << base << conversationDBID << chatname << chatDBID << callDBID;
}

void SkypeConversationProvenance::writeToDebugStreamImpl(QDebug stream) const {
    stream << "SkypeConversation(from=" << base << ", convo_dbid=" << conversationDBID;
    if (!chatname.isEmpty()) {
        stream << ", chatname=" << chatname;
    }
    if (chatDBID) {
        stream << ", chat_dbid=" << chatDBID;
    }
    if (callDBID) {
        stream << ", call_dbid=" << callDBID;
    }
    stream << ")";
}

}}}
