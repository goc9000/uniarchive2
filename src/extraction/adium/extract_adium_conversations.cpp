/**
 * extraction/adium/extract_adium_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>
#include <QDir>
#include <QFileInfo>
#include <QIODevice>
#include <QRegularExpression>

#include "extraction/adium/extract_adium_conversations.h"
#include "intermediate_format/content/TextSection.h"
#include "intermediate_format/events/IntermediateFormatEvent.h"
#include "intermediate_format/events/IFMessageEvent.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "protocols/FullAccountName.h"
#include "protocols/IMProtocols.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/language/invariant.h"

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;

namespace uniarchive2 { namespace extraction { namespace adium {

IntermediateFormatConversation init_conversation(IMM(QString) filename);


IntermediateFormatConversation extract_adium_conversation(IMM(QString) filename) {
    IntermediateFormatConversation conversation = init_conversation(filename);

    return conversation;
}

IntermediateFormatConversation init_conversation(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", qUtf8Printable(filename));

    QString full_filename = file_info.absoluteFilePath();

    IntermediateFormatConversation conversation(ArchiveFormats::ADIUM, IMProtocols::INVALID);

    conversation.originalFilename = full_filename;
    conversation.fileLastModifiedTime = ApparentTime(
        file_info.lastModified().toTime_t(),
        ApparentTime::Reference::UNKNOWN
    );

    return conversation;
}

}}}
