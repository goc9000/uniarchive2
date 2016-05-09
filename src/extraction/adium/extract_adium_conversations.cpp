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
#include "protocols/parse_account_generic.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/language/invariant.h"

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;

namespace uniarchive2 { namespace extraction { namespace adium {

struct InfoFromFilename {
    FullAccountName identity;
    FullAccountName peer;
};

IntermediateFormatConversation init_conversation(IMM(QString) filename);
InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename);
IMProtocols parse_protocol(IMM(QString) protocol_name);


IntermediateFormatConversation extract_adium_conversation(IMM(QString) filename) {
    IntermediateFormatConversation conversation = init_conversation(filename);

    return conversation;
}

IntermediateFormatConversation init_conversation(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", qUtf8Printable(filename));

    QString full_filename = file_info.absoluteFilePath();
    auto info = analyze_conversation_filename(full_filename);

    IntermediateFormatConversation conversation(ArchiveFormats::ADIUM, info.identity.protocol);

    conversation.originalFilename = full_filename;
    conversation.fileLastModifiedTime = ApparentTime(
        file_info.lastModified().toTime_t(),
        ApparentTime::Reference::UNKNOWN
    );
    conversation.identity = make_unique<SubjectGivenAsAccount>(info.identity);

    return conversation;
}

InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename) {
    InfoFromFilename info;

    QString logs_folder = full_filename.section(QDir::separator(), -5, -5);
    QString protocol_and_identity_folder = full_filename.section(QDir::separator(), -4, -4);
    QString base_name = full_filename.section(QDir::separator(), -1, -1);

    invariant(
        logs_folder == "Logs",
        "Adium archive file should be 4 levels deep under a \"Logs\" folder"
    );

    static QRegularExpression protocol_and_identity_pattern("^([^.]+)[.](.*)$");
    auto proto_and_id_match = protocol_and_identity_pattern.match(protocol_and_identity_folder);
    invariant(
        proto_and_id_match.hasMatch(),
        "Expected folder to match Protocol.account_name, but found \"%s\"",
        qUtf8Printable(protocol_and_identity_folder)
    );

    IMProtocols protocol = parse_protocol(proto_and_id_match.captured(1));
    info.identity = parse_account_generic(protocol, proto_and_id_match.captured(2));
    return info;
}

IMProtocols parse_protocol(IMM(QString) protocol_name) {
    static QRegularExpression pattern("^((Yahoo!)|(Jabber))$");

    switch (pattern.match(protocol_name).lastCapturedIndex() - 2) {
        case 0: return IMProtocols::YAHOO;
        case 1: return IMProtocols::JABBER;
        default:
            invariant_violation("Unsupported protocol specified in Adium: \"%s\"", qUtf8Printable(protocol_name));
    };
}

}}}
