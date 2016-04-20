/**
 * intermediate_format/IntermediateFormatConversation.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/IntermediateFormatConversation.h"

namespace uniarchive2 { namespace intermediate_format {

IntermediateFormatConversation::IntermediateFormatConversation()
    : originalArchiveFormat(ArchiveFormats::INVALID),  protocol(IMProtocols::INVALID) {
}

IntermediateFormatConversation::IntermediateFormatConversation(ArchiveFormats format, IMProtocols protocol)
    : originalArchiveFormat(format), protocol(protocol) {
}

QDebug operator<< (QDebug stream, const IntermediateFormatConversation& convo) {
#define N_CONTEXT_LINES 3
    QDebugStateSaver settings(stream);
    stream.nospace() << "IntermediateFormatConversation {\n";

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
    for (const auto& account : convo.declaredPeers) {
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
    if ((bool)convo.originalFilename) {
        stream << "\tOriginal filename: " << *convo.originalFilename << "\n";
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

    stream << "\t\n";
    stream << "\t" << convo.events.size() << " events:\n";
    for (const auto& event : convo.events) {
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
