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

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

MSNConversationProvenance::MSNConversationProvenance(TAKE(Provenance) base, uint session_id)
    : base(move(base)), sessionID(session_id) {
}

CEDE(Provenance) MSNConversationProvenance::clone() const {
    return make_unique<MSNConversationProvenance>(base->clone(), sessionID);
}

void MSNConversationProvenance::writeToDebugStreamImpl(QDebug stream) const {
    stream << "MSNConversation(from=" << base << ", session_id=" << sessionID << ")";
}

}}}
