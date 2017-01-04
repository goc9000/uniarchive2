/**
 * intermediate_format/provenance/MSNConversationProvenance.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/provenance/MSNConversationProvenance.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

MSNConversationProvenance::MSNConversationProvenance(TAKE(Provenance) base, uint32_t session_id)
    : base(move(base)), sessionID(session_id) {
}

ProvenanceSubType MSNConversationProvenance::subType() const {
    return ProvenanceSubType::MSN_CONVERSATION;
}

CEDE(Provenance) MSNConversationProvenance::clone() const {
    return make_unique<MSNConversationProvenance>(base->clone(), sessionID);
}

CEDE(MSNConversationProvenance) MSNConversationProvenance::deserializeFromStream(
    QDataStream& mut_stream,
    bool skip_type
) {
    maybeDeserializeType(skip_type, mut_stream, ProvenanceSubType::MSN_CONVERSATION);

    return make_unique<MSNConversationProvenance>(
        must_deserialize(mut_stream, unique_ptr<Provenance>),
        must_deserialize(mut_stream, uint32_t)
    );
}

void MSNConversationProvenance::serializeToStreamImpl(QDataStream &mut_stream) const {
    mut_stream << base << sessionID;
}

void MSNConversationProvenance::writeToDebugStreamImpl(QDebug stream) const {
    stream << "MSNConversation(from=" << base << ", session_id=" << sessionID << ")";
}

}}}
