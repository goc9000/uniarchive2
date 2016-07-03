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

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

SkypeConversationProvenance::SkypeConversationProvenance(TAKE(Provenance) base, uint64_t convo_dbid, QString chatname)
    : base(move(base)), conversationDBID(convo_dbid), chatname(chatname) {
}

CEDE(Provenance) SkypeConversationProvenance::clone() const {
    return make_unique<SkypeConversationProvenance>(base->clone(), conversationDBID, chatname);
}

void SkypeConversationProvenance::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace() << "SkypeConversation(";
    stream << "from=" << base.get();
    stream << ", convo_dbid=" << conversationDBID;
    if (!chatname.isEmpty()) {
        stream << ", chatname=" << chatname;
    }
    stream << ")";
}

}}}
