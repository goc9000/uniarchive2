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
#include <QIODevice>
#include <QRegularExpression>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>

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
#include "utils/text/decoding.h"

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;
using namespace uniarchive2::protocols::digsby;
using namespace uniarchive2::protocols::jabber;
using namespace uniarchive2::protocols::msn;
using namespace uniarchive2::protocols::yahoo;
using namespace uniarchive2::utils::text;

namespace uniarchive2 { namespace extraction { namespace digsby {

struct InfoFromFilename {
    FullAccountName identity;
    FullAccountName peer;
    bool isConference;
};

IntermediateFormatConversation init_conversation(IMM(QString) filename);
InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename);
IMProtocols parse_protocol(IMM(QString) protocol_name);
FullAccountName parse_account(IMProtocols protocol, IMM(QString) account_name);

void verify_xml_header(QTextStream& mut_stream);
void seek_start_of_events(QTextStream& mut_stream);
QStringList partially_parse_events(QTextStream& mut_stream);


IntermediateFormatConversation extract_digsby_conversation(IMM(QString) filename) {
    IntermediateFormatConversation conversation = init_conversation(filename);

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal("Can't open file: %s", qUtf8Printable(filename));
    }

    // Unfortunately we are reduced to manual text processing as the HTML produced by Digsby is occasionally damaged
    QTextStream stream(&file);

    verify_xml_header(stream);
    seek_start_of_events(stream);
    QStringList event_htmls = partially_parse_events(stream);

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
    conversation.isConference = info.isConference;
    conversation.identity = make_unique<SubjectGivenAsAccount>(info.identity);
    conversation.declaredPeers.push_back(make_unique<SubjectGivenAsAccount>(info.peer));

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

    if (peer_folder == "Group Chats") {
        info.isConference = true;
        static QRegularExpression pat_group_chat(
            "^\\d{4}-\\d{2}-\\d{2}T\\d{2}[.]\\d{2}[.]\\d{2} - (.*)-\\d+[.]html$",
            QRegularExpression::CaseInsensitiveOption
        );
        auto group_chat_match = pat_group_chat.match(base_name);
        invariant(
            group_chat_match.hasMatch(),
            "Digsby conference archive file must have a filename like "
                "YYYY-mm-ddThh.mm.ss - account_name-<timestamp>.html, it is actually \"%s\"",
            qUtf8Printable(base_name)
        );

        info.peer = parse_account(protocol, group_chat_match.captured(1));
    } else {
        info.isConference = false;
        static QRegularExpression pat_base_name(
            "^\\d{4}-\\d{2}-\\d{2}.html$",
            QRegularExpression::CaseInsensitiveOption
        );
        invariant(
            pat_base_name.match(base_name).hasMatch(),
            "Digsby archive file must have a filename like YYYY-mm-dd.html, it is actually \"%s\"",
            qUtf8Printable(base_name)
        );

        static QRegularExpression pat_remote_peer("^(.*)_([a-z]+)$", QRegularExpression::CaseInsensitiveOption);
        auto remote_peer_match = pat_remote_peer.match(peer_folder);
        invariant(
            remote_peer_match.hasMatch(),
            "Digsby peer folder must look like <account>_<protocol>, it is actually \"%s\"",
            qUtf8Printable(peer_folder)
        );

        IMProtocols peer_protocol = parse_protocol(remote_peer_match.captured(2));
        info.peer = parse_account(peer_protocol, remote_peer_match.captured(1));
    }

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

void verify_xml_header(QTextStream& mut_stream) {
    QString xml_header = mut_stream.readLine();
    invariant(
        xml_header.startsWith("<?xml ") && xml_header.contains("encoding=\"UTF-8\""),
        "Expected Digsby archive to be a UTF-8 encoded XML file, but header is \"%s\"",
        qUtf8Printable(xml_header)
    );

    mut_stream.setCodec(QTextCodec::codecForName("UTF-8"));
}

void seek_start_of_events(QTextStream& mut_stream) {
    while (!mut_stream.atEnd()) {
        QString line = mut_stream.readLine();

        if (line.trimmed() == "<BODY onload=\"utc_to_local()\">") {
            return;
        }
    }

    invariant_violation(
        "Could not find the <BODY onload=\"utc_to_local()\"> sequence that marks the start of messages"
    );
}

QStringList partially_parse_events(QTextStream& mut_stream) {
    QStringList event_htmls;
    QString current_event_html;
    bool event_open = false;

    while (!mut_stream.atEnd()) {
        QString line = mut_stream.readLine();

        if (line.startsWith("<div class=\"")) {
            if (event_open) {
                event_htmls << current_event_html;
            }
            current_event_html = line;
            event_open = true;
        } else if (event_open) {
            current_event_html.append("\n").append(line);
        } else {
            event_htmls << line;
            continue;
        }

        if (line.endsWith("</span></div>")) {
            event_htmls << current_event_html;
            event_open = false;
        }
    }

    if (event_open) {
        event_htmls << current_event_html;
    }

    return event_htmls;
}

}}}
