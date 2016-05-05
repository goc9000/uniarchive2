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
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>

#include "extraction/digsby/extract_digsby_conversations.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "protocols/digsby/account_name.h"
#include "protocols/jabber/account_name.h"
#include "protocols/msn/account_name.h"
#include "protocols/yahoo/account_name.h"
#include "protocols/FullAccountName.h"
#include "protocols/IMProtocols.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/language/invariant.h"

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;
using namespace uniarchive2::protocols::digsby;
using namespace uniarchive2::protocols::jabber;
using namespace uniarchive2::protocols::msn;
using namespace uniarchive2::protocols::yahoo;

namespace uniarchive2 { namespace extraction { namespace digsby {

struct InfoFromFilename {
    FullAccountName identity;
};

IntermediateFormatConversation init_conversation(IMM(QString) filename);
InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename);
IMProtocols parse_protocol(IMM(QString) protocol_name);
FullAccountName parse_account(IMProtocols protocol, IMM(QString) account_name);


IntermediateFormatConversation extract_digsby_conversation(IMM(QString) filename) {
    IntermediateFormatConversation conversation = init_conversation(filename);

    return conversation;
}

IntermediateFormatConversation init_conversation(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", qUtf8Printable(filename));

    QString full_filename = file_info.absoluteFilePath();
    auto info = analyze_conversation_filename(full_filename);

    IntermediateFormatConversation conversation(ArchiveFormats::DIGSBY, info.identity.protocol);

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

    QString logs_folder = full_filename.section(QDir::separator(), -6, -6);
    QString protocol_folder = full_filename.section(QDir::separator(), -4, -4);
    QString identity_folder = full_filename.section(QDir::separator(), -3, -3);
    QString peer_folder = full_filename.section(QDir::separator(), -2, -2);
    QString base_name = full_filename.section(QDir::separator(), -1, -1);

    invariant(
        logs_folder == "Digsby Logs",
        "Digsby archive file should be 5 levels deep under a \"Digsby Logs\" folder"
    );

    IMProtocols protocol = parse_protocol(protocol_folder);
    info.identity = parse_account(protocol, identity_folder);

    return info;
}

IMProtocols parse_protocol(IMM(QString) protocol_name) {
    static QRegularExpression pattern("^((digsby)|(gtalk|jabber)|(msn)|(yahoo))$");

    switch (pattern.match(protocol_name).lastCapturedIndex() - 2) {
        case 0: return IMProtocols::DIGSBY;
        case 1: return IMProtocols::JABBER;
        case 2: return IMProtocols::MSN;
        case 3: return IMProtocols::YAHOO;
        default:
            invariant_violation("Unsupported protocol specified in Digsby: \"%s\"", qUtf8Printable(protocol_name));
    };
}

FullAccountName parse_account(IMProtocols protocol, IMM(QString) account_name) {
    switch (protocol) {
        case IMProtocols::DIGSBY: return parse_digsby_account(account_name);
        case IMProtocols::JABBER: return parse_jabber_account(account_name);
        case IMProtocols::MSN: return parse_msn_account(account_name);
        case IMProtocols::YAHOO: return parse_yahoo_account(account_name);
        default:
            invariant_violation(
                "Unsupported protocol for parsing an account in Digsby: %s",
                qUtf8Printable(name_for_im_protocol(protocol))
            );
    }
}

}}}
