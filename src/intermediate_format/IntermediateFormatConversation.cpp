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
    : originalArchiveFormat(ArchiveFormats::INVALID), localAccount(FullAccountName(IMProtocols::INVALID, "")) {
}

IntermediateFormatConversation::IntermediateFormatConversation(
    ArchiveFormats format,
    FullAccountName local_account
): originalArchiveFormat(format), localAccount(local_account) {
}

QDebug operator<< (QDebug stream, const IntermediateFormatConversation& convo) {
#define N_CONTEXT_LINES 3
    QDebugStateSaver settings(stream);
    stream.nospace() << "IntermediateFormatConversation {\n";

    stream << "\tFormat: " << convo.originalArchiveFormat << "\n";
    stream << "\tLocal account: " << convo.localAccount << "\n";

    stream << "\tDeclared remote accounts: [";
    bool first = true;
    for (auto account : convo.declaredRemoteAccounts) {
        if (!first) {
            stream << ", ";
        }
        stream << account;
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

    stream << "\t\n";
    stream << "\t" << convo.events.length() << " events:\n";
    for (const auto& event : convo.events) {
        if ((event->indexInConversation >= N_CONTEXT_LINES) &&
            (event->indexInConversation < convo.events.length() - N_CONTEXT_LINES)) {
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
