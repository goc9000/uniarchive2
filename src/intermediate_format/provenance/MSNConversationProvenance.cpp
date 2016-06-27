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

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

MSNConversationProvenance::MSNConversationProvenance(
    TAKE(Provenance) base,
    unsigned int session_id
) : base(move(base)), sessionID(session_id) {
}

CEDE(Provenance) MSNConversationProvenance::clone() const {
    return make_unique<MSNConversationProvenance>(base->clone(), sessionID);
}

void MSNConversationProvenance::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace() << "MSNConversation(";
    stream << "from=" << base.get();
    stream << ", session_id=" << sessionID;
    stream << ")";
}

}}}
