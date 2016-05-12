/**
 * intermediate_format/RawConversation.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/RawConversation.h"

namespace uniarchive2 { namespace intermediate_format {

RawConversation::RawConversation()
    : originalArchiveFormat(ArchiveFormat::INVALID),  protocol(IMProtocol::INVALID) {
}

RawConversation::RawConversation(ArchiveFormat format, IMProtocol protocol)
    : originalArchiveFormat(format), protocol(protocol) {
}

RawConversation RawConversation::fromPrototype(IMM(RawConversation) prototype) {
    RawConversation convo(prototype.originalArchiveFormat, prototype.protocol);

    if (prototype.identity) {
        convo.identity = prototype.identity->clone();
    }
    for (IMM(auto) subject : prototype.declaredPeers) {
        convo.declaredPeers.push_back(subject->clone());
    }

    convo.isConference = prototype.isConference;
    convo.declaredStartDate = prototype.declaredStartDate;
    convo.originalFilename = prototype.originalFilename;
    convo.fileLastModifiedTime = prototype.fileLastModifiedTime;
    convo.numConversationInFile = prototype.numConversationInFile;
    convo.conversationOffsetInFileEventBased = prototype.conversationOffsetInFileEventBased;

    return convo;
}

QDebug operator<< (QDebug stream, IMM(RawConversation) convo) {
#define N_CONTEXT_LINES 3
    QDebugStateSaver settings(stream);
    stream.nospace() << "RawConversation {\n";

    stream << "\tFormat: " << convo.originalArchiveFormat << "\n";
    stream << "\tProtocol: " << convo.protocol << "\n";

    stream << "\tIdentity: ";
    if (convo.identity) {
        stream << convo.identity.get();
    } else {
        stream << "(unspecified)";
    }
    stream << "\n";

    stream << "\tDeclared peers: [";
    bool first = true;
    for (IMM(auto) account : convo.declaredPeers) {
        if (!first) {
            stream << ", ";
        }
        stream << account.get();
        first = false;
    }
    stream << "]\n";

    if ((bool)convo.isConference) {
        stream << "\tIs conference: " << *convo.isConference << "\n";
    }
    if ((bool)convo.declaredStartDate) {
        stream << "\tDeclared start date: " << *convo.declaredStartDate << "\n";
    }
    if (!convo.originalFilename.isEmpty()) {
        stream << "\tOriginal filename: " << convo.originalFilename << "\n";
    }
    if ((bool)convo.fileLastModifiedTime) {
        stream << "\tFile last modified time: " << *convo.fileLastModifiedTime << "\n";
    }
    if ((bool)convo.numConversationInFile) {
        stream << "\tNb. conversation in file: " << *convo.numConversationInFile << "\n";
    }
    if ((bool)convo.conversationOffsetInFileEventBased) {
        stream << "\tConversation starts at event: " << *convo.conversationOffsetInFileEventBased << "\n";
    }
    if (!convo.adiumVersion.isEmpty()) {
        stream << "\tAdium version: " << convo.adiumVersion << "\n";
    }
    if (!convo.adiumBuildID.isEmpty()) {
        stream << "\tAdium build ID: " << convo.adiumBuildID << "\n";
    }

    stream << "\t\n";
    stream << "\t" << convo.events.size() << " events:\n";
    for (IMM(auto) event : convo.events) {
        if ((event->indexInConversation >= N_CONTEXT_LINES) &&
            (event->indexInConversation < convo.events.size() - N_CONTEXT_LINES)) {
            if (event->indexInConversation == N_CONTEXT_LINES) {
                stream << "\t...\n";
            }
            continue;
        }
        stream << "\t" << event.get() << "\n";
    }

    stream << "}";

    return stream;
}

}}
