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
#include "utils/serialization/serialization_helpers.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/qt/debug_extras.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format {

RawConversation::RawConversation() : protocol(IMProtocol::INVALID) {
    // Nothing else to initialize
}

RawConversation::RawConversation(IMProtocol protocol) : protocol(protocol) {
    // Nothing else to initialize
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

RawConversation RawConversation::deserializeFromStream(QDataStream& mut_stream) {
    RawConversation convo(must_deserialize(mut_stream, IMProtocol));

    convo.identity = must_deserialize_optional_unique_ptr(mut_stream, ApparentSubject);

    mut_stream >> convo.declaredPeers >> convo.isConference >> convo.conferenceTitle >> convo.declaredStartDate;

    convo.declaredInitiator = must_deserialize_optional_unique_ptr(mut_stream, ApparentSubject);
    convo.provenance = must_deserialize_optional_unique_ptr(mut_stream, Provenance);

    mut_stream >> convo.events;

    return convo;
}

void RawConversation::serializeToStream(QDataStream& mut_stream) const {
    mut_stream << protocol;
    serialize_optional_unique_ptr(mut_stream, identity);
    mut_stream << declaredPeers << isConference << conferenceTitle << declaredStartDate;
    serialize_optional_unique_ptr(mut_stream, declaredInitiator);
    serialize_optional_unique_ptr(mut_stream, provenance);
    mut_stream << events;
}

void RawConversation::writeToDebugStream(QDebug stream, bool all_messages) const {
#define N_CONTEXT_LINES 3
    QDebugStateSaver settings(stream);
    stream.nospace() << "RawConversation {\n";

    stream << "\tProtocol: " << protocol << "\n";

    stream << "\tIdentity: ";
    if (identity) {
        stream << identity;
    } else {
        stream << "(unspecified)";
    }
    stream << "\n";

    stream << "\tDeclared peers: " << declaredPeers << "\n";

    if (isConference) {
        stream << "\tIs conference: " << isConference << "\n";
    }
    if (declaredInitiator) {
        stream << "\tInitiator: " << declaredInitiator << "\n";
    }
    if (conferenceTitle) {
        stream << "\tConference title: " << conferenceTitle << "\n";
    }
    if (declaredStartDate) {
        stream << "\tDeclared start date: " << declaredStartDate << "\n";
    }

    if (provenance) {
        stream << "\tProvenance: " << provenance << "\n";
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

    stream << "}" << "\n";
}

QDebug operator<< (QDebug stream, IMM(RawConversation) convo) {
    convo.writeToDebugStream(stream);
    return stream;
}

}}
