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
    convo.conferenceTitle = prototype.conferenceTitle;
    convo.declaredStartDate = prototype.declaredStartDate;
    convo.originalFilename = prototype.originalFilename;
    convo.fileLastModifiedTime = prototype.fileLastModifiedTime;
    convo.numConversationInFile = prototype.numConversationInFile;
    convo.conversationOffsetInFileEventBased = prototype.conversationOffsetInFileEventBased;
    convo.adiumVersion = prototype.adiumVersion;
    convo.adiumBuildID = prototype.adiumBuildID;
    convo.shouldBeRepartitioned = prototype.shouldBeRepartitioned;

    return convo;
}

void RawConversation::writeToDebugStream(QDebug stream, bool all_messages) const {
#define N_CONTEXT_LINES 3
    QDebugStateSaver settings(stream);
    stream.nospace() << "RawConversation {\n";

    stream << "\tFormat: " << originalArchiveFormat << "\n";
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
    if (!conferenceTitle.isEmpty()) {
        stream << "\tConference title: " << conferenceTitle << "\n";
    }
    if ((bool)declaredStartDate) {
        stream << "\tDeclared start date: " << *declaredStartDate << "\n";
    }
    if (!originalFilename.isEmpty()) {
        stream << "\tOriginal filename: " << originalFilename << "\n";
    }
    if ((bool)fileLastModifiedTime) {
        stream << "\tFile last modified time: " << *fileLastModifiedTime << "\n";
    }
    if ((bool)numConversationInFile) {
        stream << "\tNb. conversation in file: " << *numConversationInFile << "\n";
    }
    if ((bool)conversationOffsetInFileEventBased) {
        stream << "\tConversation starts at event: " << *conversationOffsetInFileEventBased << "\n";
    }
    if (!adiumVersion.isEmpty()) {
        stream << "\tAdium version: " << adiumVersion << "\n";
    }
    if (!adiumBuildID.isEmpty()) {
        stream << "\tAdium build ID: " << adiumBuildID << "\n";
    }
    if ((bool)shouldBeRepartitioned) {
        stream << "\tShould be repartitioned: " << *shouldBeRepartitioned << "\n";
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
