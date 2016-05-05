/**
 * extraction/digsby/extract_digsby_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>
#include <QFileInfo>

#include "extraction/digsby/extract_digsby_conversations.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/language/invariant.h"

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::protocols;

namespace uniarchive2 { namespace extraction { namespace digsby {

IntermediateFormatConversation init_conversation(IMM(QString) filename);


IntermediateFormatConversation extract_digsby_conversation(IMM(QString) filename) {
    IntermediateFormatConversation conversation = init_conversation(filename);

    return conversation;
}

IntermediateFormatConversation init_conversation(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", qUtf8Printable(filename));

    QString full_filename = file_info.absoluteFilePath();

    IntermediateFormatConversation conversation(ArchiveFormats::DIGSBY, IMProtocols::INVALID);

    conversation.originalFilename = full_filename;
    conversation.fileLastModifiedTime = ApparentTime(
        file_info.lastModified().toTime_t(),
        ApparentTime::Reference::UNKNOWN
    );

    return conversation;
}

}}}
