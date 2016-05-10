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
#include "intermediate_format/events/IFUninterpretedEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/subjects/FullySpecifiedSubject.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "protocols/FullAccountName.h"
#include "protocols/IMProtocols.h"
#include "protocols/parse_account_generic.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/language/invariant.h"
#include "utils/xml/qdom_utils.h"

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;
using namespace uniarchive2::utils::xml;

namespace uniarchive2 { namespace extraction { namespace adium {

struct InfoFromFilename {
    FullAccountName identity;
    FullAccountName peer;
    ApparentTime convoStartDate;
};

IntermediateFormatConversation init_conversation(IMM(QString) filename);
InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename);
IMProtocols parse_protocol(IMM(QString) protocol_name);
void verify_identity(IMM(QDomElement) root_element, IMM(FullAccountName) identity);

CEDE(IntermediateFormatEvent) parse_event(
    IMM(QDomElement) event_element,
    IMM(IntermediateFormatConversation) conversation
);
CEDE(ApparentSubject) parse_event_subject(
    IMM(QDomElement) event_element,
    IMM(IntermediateFormatConversation) conversation
);


IntermediateFormatConversation extract_adium_conversation(IMM(QString) filename) {
    IntermediateFormatConversation conversation = init_conversation(filename);

    QDomDocument xml = load_xml_file(filename);
    QDomElement root_element = get_dom_root(xml, "chat");

    verify_identity(root_element, static_cast<SubjectGivenAsAccount*>(conversation.identity.get())->account);
    conversation.adiumVersion = read_string_attr(root_element, "adiumversion");
    conversation.adiumBuildID = read_string_attr(root_element, "buildid");

    QDomElement event_element = root_element.firstChildElement();
    while (!event_element.isNull()) {
        conversation.events.push_back(parse_event(event_element, conversation));
        event_element = event_element.nextSiblingElement();
    }

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
    conversation.declaredPeers.push_back(make_unique<SubjectGivenAsAccount>(info.peer));
    conversation.declaredStartDate = info.convoStartDate;

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

    static QRegularExpression filename_pattern(
        "^(.*) \\((\\d{4}-\\d{2}-\\d{2}T\\d{2}.\\d{2}.\\d{2}(|Z|[+-]\\d+))\\)[.]xml$"
    );
    auto filename_match = filename_pattern.match(base_name);
    invariant(
        filename_match.hasMatch(),
        "Expected archive filename to match pattern \"account_name (YYYY-mm-ddThh.mm.ss+offset).xml\", found \"%s\"",
        qUtf8Printable(base_name)
    );

    info.peer = parse_account_generic(protocol, filename_match.captured(1));

    QDateTime datetime = QDateTime::fromString(filename_match.captured(2), Qt::ISODate);
    invariant(datetime.isValid(), "Could not parse XML date \"%s\"", qUtf8Printable(filename_match.captured(2)));

    info.convoStartDate = ApparentTime(datetime);

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

void verify_identity(IMM(QDomElement) root_element, IMM(FullAccountName) identity) {
    FullAccountName file_account(
        parse_protocol(read_string_attr(root_element, "service")),
        read_string_attr(root_element, "account")
    );

    invariant(identity == file_account, "Found an unexpected identity in the archive");
}

CEDE(IntermediateFormatEvent) parse_event(
    IMM(QDomElement) event_element,
    IMM(IntermediateFormatConversation) conversation
) {
    ApparentTime event_time = ApparentTime(read_iso_date_attr(event_element, "time"));
    unique_ptr<ApparentSubject> event_subject = parse_event_subject(event_element, conversation);

    // Temporary default
    return make_unique<IFUninterpretedEvent>(
        event_time,
        conversation.events.size(),
        xml_to_raw_data(event_element)
    );
}

CEDE(ApparentSubject) parse_event_subject(
    IMM(QDomElement) event_element,
    IMM(IntermediateFormatConversation) conversation
) {
    if (!event_element.hasAttribute("sender")) {
        return unique_ptr<ApparentSubject>();
    }

    QString account_name = read_string_attr(event_element, "sender");
    FullAccountName account(conversation.protocol, account_name);

    if (event_element.hasAttribute("alias")) {
        return make_unique<FullySpecifiedSubject>(account, read_string_attr(event_element, "alias"));
    }

    return make_unique<SubjectGivenAsAccount>(account);
}

}}}
