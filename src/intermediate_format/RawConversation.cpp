/**
 * intermediate_format/RawConversation.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/RawConversation.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format {

RawConversation::RawConversation() : protocol(IMProtocol::INVALID) {
}

RawConversation::RawConversation(IMProtocol protocol) : protocol(protocol) {
}

RawConversation RawConversation::fromPrototype(IMM(RawConversation) prototype) {
    RawConversation convo(prototype.protocol);

    if (prototype.identity) {
        convo.identity = prototype.identity->clone();
    }
    for (IMM(auto) subject : prototype.declaredPeers) {
        convo.declaredPeers.push_back(subject->clone());
    }
    if (prototype.declaredInitiator) {
        convo.declaredInitiator = prototype.declaredInitiator->clone();
    }

    convo.isConference = prototype.isConference;
    convo.conferenceTitle = prototype.conferenceTitle;
    convo.declaredStartDate = prototype.declaredStartDate;

    if (prototype.provenance) {
        convo.provenance = prototype.provenance->clone();
    }

    return convo;
}

void RawConversation::writeToDebugStream(QDebug stream, bool all_messages) const {
#define N_CONTEXT_LINES 3
    QDebugStateSaver settings(stream);
    stream.nospace() << "RawConversation {\n";

    stream << "\tProtocol: " << protocol << "\n";

    stream << "\tIdentity: ";
    if (identity) {
        stream << identity.get();
    } else {
        stream << "(unspecified)";
    }
    stream << "\n";

    stream << "\tDeclared peers: [";
    bool first = true;
    for (IMM(auto) account : declaredPeers) {
        if (!first) {
            stream << ", ";
        }
        stream << account.get();
        first = false;
    }
    stream << "]\n";

    if ((bool)isConference) {
        stream << "\tIs conference: " << *isConference << "\n";
    }
    if ((bool)declaredInitiator) {
        stream << "\tInitiator: " << declaredInitiator.get() << "\n";
    }
    if (!conferenceTitle.isEmpty()) {
        stream << "\tConference title: " << conferenceTitle << "\n";
    }
    if ((bool)declaredStartDate) {
        stream << "\tDeclared start date: " << *declaredStartDate << "\n";
    }

    if (provenance) {
        stream << "\tProvenance: " << provenance.get() << "\n";
    }

    stream << "\t\n";
    stream << "\t" << events.size() << " events:\n";
    for (IMM(auto) event : events) {
        if (!all_messages &&
            (event->indexInConversation >= N_CONTEXT_LINES) &&
            (event->indexInConversation < events.size() - N_CONTEXT_LINES)) {
            if (event->indexInConversation == N_CONTEXT_LINES) {
                stream << "\t...\n";
            }
            continue;
        }
        stream << "\t" << event.get() << "\n";
    }

    stream << "}";
}

QDebug operator<< (QDebug stream, IMM(RawConversation) convo) {
    convo.writeToDebugStream(stream);
    return stream;
}

}}
