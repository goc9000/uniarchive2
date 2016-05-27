/**
 * extraction/whatsapp/extract_whatsapp_email_conversations.cpp
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

#include "extraction/whatsapp/extract_whatsapp_email_conversations.h"
#include "protocols/ArchiveFormat.h"
#include "protocols/IMProtocol.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

using namespace std;
using namespace uniarchive2::protocols;

namespace uniarchive2 { namespace extraction { namespace whatsapp {

static RawConversation init_conversation(IMM(QString) filename);


RawConversation extract_whatsapp_email_conversation(IMM(QString)filename) {
    RawConversation conversation = init_conversation(filename);

    return conversation;
}

static RawConversation init_conversation(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", QP(filename));

    QString full_filename = file_info.absoluteFilePath();

    QREGEX_MUST_MATCH(
        name_match, "^WhatsApp Chat with .+[.]txt$", full_filename.section(QDir::separator(), -1, -1),
        "WhatApp conversation filename should be 'WhatSapp Chat with Peer Name.txt', is instead '%s'"
    );

    RawConversation conversation(ArchiveFormat::WHATSAPP_EMAIL, IMProtocol::WHATSAPP);
    conversation.originalFilename = full_filename;
    conversation.fileLastModifiedTime = ApparentTime::fromQDateTimeUnknownReference(file_info.lastModified());

    return conversation;
}

}}}