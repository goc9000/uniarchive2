/**
 * extraction/adium/extract_adium_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "extraction/adium/extract_adium_conversations.h"
#include "extraction/parse_libpurple_system_message.h"
#include "intermediate_format/content/formatting/CSSStyleTag.h"
#include "intermediate_format/content/formatting/LinkTag.h"
#include "intermediate_format/content/LineBreak.h"
#include "intermediate_format/content/TextSection.h"
#include "intermediate_format/events/status/RawDisconnectedEvent.h"
#include "intermediate_format/events/status/RawConnectedEvent.h"
#include "intermediate_format/events/status/RawStatusChangeEvent.h"
#include "intermediate_format/events/system/RawWindowClosedEvent.h"
#include "intermediate_format/events/system/RawWindowOpenedEvent.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/events/RawMessageEvent.h"
#include "intermediate_format/events/RawPingEvent.h"
#include "intermediate_format/events/RawUninterpretedEvent.h"
#include "intermediate_format/provenance/AdiumArchiveFileProvenance.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/subjects/ImplicitSubject.h"
#include "intermediate_format/subjects/FullySpecifiedSubject.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "intermediate_format/subjects/SubjectGivenAsScreenName.h"
#include "protocols/FullAccountName.h"
#include "protocols/IMProtocol.h"
#include "protocols/IMStatus.h"
#include "protocols/parse_account_generic.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/xml/qdom_utils.h"

#include <deque>

#include <QtDebug>
#include <QDir>
#include <QFileInfo>
#include <QIODevice>
#include <QMap>
#include <QUrl>

namespace uniarchive2 { namespace extraction { namespace adium {

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::provenance;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;
using namespace uniarchive2::utils::xml;

static const QMap<QString, IMStatus> EVENT_TYPE_TO_STATUS {
    { "available", IMStatus::AVAILABLE },
    { "away", IMStatus::AWAY },
    { "idle", IMStatus::IDLE },
    { "offline", IMStatus::OFFLINE },
    { "online", IMStatus::AVAILABLE },
};

struct InfoFromFilename {
    FullAccountName identity;
    FullAccountName peer;
    ApparentTime convoStartDate;
};

static RawConversation init_conversation(IMM(QString) filename);
static InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename);
static IMProtocol parse_protocol(IMM(QString) protocol_name);
static void verify_identity(IMM(QDomElement) root_element, IMM(FullAccountName) identity);

static CEDE(RawEvent) parse_event(IMM(QDomElement) event_element, IMM(RawConversation) conversation);
static CEDE(ApparentSubject) parse_event_subject(IMM(QDomElement) event_element, IMM(RawConversation) conversation);
static CEDE(RawEvent) parse_system_event(IMM(QDomElement) event_element, ApparentTime event_time, uint event_index);
static CEDE(RawMessageEvent) parse_message_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    uint event_index,
    TAKE(ApparentSubject) event_subject
);
static CEDE(RawEvent) parse_status_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    uint event_index,
    TAKE(ApparentSubject) event_subject,
    IMProtocol protocol
);
static CEDE(RawStatusChangeEvent) parse_status_change_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    uint event_index,
    TAKE(ApparentSubject) event_subject
);
static RawMessageContent parse_event_content(IMM(QDomElement) event_element);
static void parse_event_content_rec(IMM(QDomElement) element, RawMessageContent& mut_content);
static CEDE(RawMessageContentItem) convert_event_content_open_tag(IMM(QDomElement) element);
static CEDE(RawMessageContentItem) convert_event_content_closed_tag(IMM(QDomElement) element);
static CEDE(RawMessageContentItem) convert_event_content_open_tag_secondary(IMM(QDomElement) element);
static CEDE(RawMessageContentItem) convert_event_content_closed_tag_secondary(IMM(QDomElement) element);
static CEDE(TextSection) convert_event_content_text(IMM(QDomText) text_node);


RawConversation extract_adium_conversation(IMM(QString) filename) {
    RawConversation conversation = init_conversation(filename);

    QDomDocument xml = load_xml_file(filename);
    QDomElement root_element = get_dom_root(xml, "chat");

    auto provenance = conversation.provenance->as<AdiumArchiveFileProvenance>();
    provenance->adiumVersion = read_string_attr(root_element, "adiumversion");
    provenance->adiumBuildID = read_string_attr(root_element, "buildid");

    verify_identity(root_element, conversation.identity->as<SubjectGivenAsAccount>()->account);

    QDomElement event_element = root_element.firstChildElement();
    while (!event_element.isNull()) {
        conversation.events.push_back(parse_event(event_element, conversation));
        event_element = event_element.nextSiblingElement();
    }

    return conversation;
}

static RawConversation init_conversation(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", QP(filename));

    QString full_filename = file_info.absoluteFilePath();
    auto info = analyze_conversation_filename(full_filename);

    RawConversation conversation(info.identity.protocol);
    conversation.provenance = AdiumArchiveFileProvenance::fromQFileInfo(file_info);

    conversation.identity = make_unique<SubjectGivenAsAccount>(info.identity);
    conversation.declaredPeers.push_back(make_unique<SubjectGivenAsAccount>(info.peer));
    conversation.declaredStartDate = info.convoStartDate;

    return conversation;
}

static InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename) {
    InfoFromFilename info;

    QString logs_folder = full_filename.section(QDir::separator(), -5, -5);
    QString protocol_and_identity_folder = full_filename.section(QDir::separator(), -4, -4);
    QString base_name = full_filename.section(QDir::separator(), -1, -1);

    invariant(
        logs_folder == "Logs",
        "Adium archive file should be 4 levels deep under a \"Logs\" folder"
    );

    QREGEX_MUST_MATCH(
        proto_and_id_match, "^([^.]+)[.](.*)$", protocol_and_identity_folder,
        "Expected folder to match Protocol.account_name, but found \"%s\""
    );

    IMProtocol protocol = parse_protocol(proto_and_id_match.captured(1));
    info.identity = parse_account_generic(protocol, proto_and_id_match.captured(2));

    QREGEX_MUST_MATCH(
        filename_match, "^(.*) \\((\\d{4}-\\d{2}-\\d{2}T\\d{2}.\\d{2}.\\d{2}(|Z|[+-]\\d+))\\)[.]xml$", base_name,
        "Expected archive filename to match pattern \"account_name (YYYY-mm-ddThh.mm.ss+offset).xml\", found \"%s\""
    );

    info.peer = parse_account_generic(protocol, filename_match.captured(1));

    QDateTime datetime = QDateTime::fromString(filename_match.captured(2), Qt::ISODate);
    invariant(datetime.isValid(), "Could not parse XML date \"%s\"", QP(filename_match.captured(2)));

    info.convoStartDate = ApparentTime::fromQDateTime(datetime);

    return info;
}

static IMProtocol parse_protocol(IMM(QString) protocol_name) {
    const static QMap<QString, IMProtocol> PROTOCOL_MAP = {
        { "Yahoo!", IMProtocol::YAHOO },
        { "GTalk", IMProtocol::JABBER },
        { "Jabber", IMProtocol::JABBER },
    };

    if (PROTOCOL_MAP.contains(protocol_name)) {
        return PROTOCOL_MAP[protocol_name];
    }

    invariant_violation("Unrecognized protocol in Adium: \"%s\"", QP(protocol_name));
}

static void verify_identity(IMM(QDomElement) root_element, IMM(FullAccountName) identity) {
    FullAccountName file_account(
        parse_protocol(read_string_attr(root_element, "service")),
        read_string_attr(root_element, "account")
    );

    invariant(identity == file_account, "Found an unexpected identity in the archive");
}

static CEDE(RawEvent) parse_event(IMM(QDomElement) event_element, IMM(RawConversation) conversation) {
    ApparentTime event_time = ApparentTime::fromQDateTime(read_iso_date_attr(event_element, "time"));
    unique_ptr<ApparentSubject> event_subject = parse_event_subject(event_element, conversation);
    uint event_index = (uint)conversation.events.size();

    if (event_element.tagName() == "event") {
        return parse_system_event(event_element, event_time, event_index);
    } else if (event_element.tagName() == "message") {
        return parse_message_event(event_element, event_time, event_index, move(event_subject));
    } else if (event_element.tagName() == "status") {
        return parse_status_event(event_element, event_time, event_index, move(event_subject), conversation.protocol);
    }

    invariant_violation("Unsupported event tag <%s>", QP(event_element.tagName()));
}

static CEDE(ApparentSubject) parse_event_subject(IMM(QDomElement) event_element, IMM(RawConversation) conversation) {
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

static CEDE(RawEvent) parse_system_event(IMM(QDomElement) event_element, ApparentTime event_time, uint event_index) {
    QString event_type = read_string_attr(event_element, "type");

    if (event_type == "windowOpened") {
        return make_unique<RawWindowOpenedEvent>(event_time, event_index);
    } else if (event_type == "windowClosed") {
        return make_unique<RawWindowClosedEvent>(event_time, event_index);
    }

    invariant_violation("Unsupported <event> type: %s", QP(event_type));
}

static CEDE(RawMessageEvent) parse_message_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    uint event_index,
    TAKE(ApparentSubject) event_subject
) {
    return make_unique<RawMessageEvent>(
        event_time,
        event_index,
        move(event_subject),
        parse_event_content(event_element)
    );
}

static void expect_event_text(IMM(QDomElement) event_element, IMM(QString) expected_text) {
    QString event_text = event_element.text().trimmed();

    invariant(
        event_text == expected_text,
        "Expected event text to be \"%s\", found \"%s\"",
        QP(expected_text),
        QP(event_text)
    );
}

static CEDE(RawEvent) parse_status_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    uint event_index,
    TAKE(ApparentSubject) event_subject,
    IMProtocol protocol
) {
    QString event_type = event_element.attribute("type");

    if (EVENT_TYPE_TO_STATUS.contains(event_type)) {
        return parse_status_change_event(event_element, event_time, event_index, move(event_subject));
    } else if (event_type == "connected") {
        expect_event_text(event_element, "You have connected");
        return make_unique<RawConnectedEvent>(
            event_time,
            event_index,
            make_unique<ImplicitSubject>(ImplicitSubjectKind::IDENTITY)
        );
    } else if (event_type == "disconnected") {
        expect_event_text(event_element, "You have disconnected");
        return make_unique<RawDisconnectedEvent>(
            event_time,
            event_index,
            make_unique<ImplicitSubject>(ImplicitSubjectKind::IDENTITY)
        );
    } else if (event_type == "Notification") {
        if (event_element.text().trimmed().endsWith(" wants your attention!")) {
            return make_unique<RawPingEvent>(event_time, event_index, move(event_subject));
        }
        invariant_violation("Unsupported Notification event: %s", QP(xml_to_string(event_element)));
    } else if ((event_type == "purple") || (event_type == "libpurpleMessage")) {
        QREGEX_WILL_MATCH(
            content_match,
            "^<[^>]*>(.*)<[^>]*>$",
            xml_to_string(only_child_elem(only_child_elem(event_element, "div"), "span"))
        );

        return parse_libpurple_system_message(
            event_index,
            event_time,
            content_match.captured(1).trimmed(),
            true,
            protocol
        );
    }

    invariant_violation("Unsupported <status> event type: %s", QP(event_type));
}

static CEDE(RawStatusChangeEvent) parse_status_change_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    uint event_index,
    TAKE(ApparentSubject) event_subject
) {
    IMStatus status = EVENT_TYPE_TO_STATUS[event_element.attribute("type")];

    unique_ptr<RawStatusChangeEvent> status_change =
        make_unique<RawStatusChangeEvent>(event_time, event_index, move(event_subject), status);

    RawMessageContent content = parse_event_content(event_element);
    if (!content.items.empty()) {
        status_change->message = move(content);
    }

    return status_change;
}

static RawMessageContent parse_event_content(IMM(QDomElement) event_element) {
    RawMessageContent content;

    if (!event_element.hasChildNodes()) {
        return content;
    }

    auto main_div = only_child_elem(event_element, "div");
    invariant(main_div.attributes().length() == 0, "Did not expect content <div> to have attributes");

    parse_event_content_rec(main_div, content);

    return content;
}

static void parse_event_content_rec(IMM(QDomElement) element, RawMessageContent& mut_content) {
    for (QDomNode node = element.firstChild(); !node.isNull(); node = node.nextSibling()) {
        if (node.isElement()) {
            QDomElement tag = node.toElement();
            mut_content.addItem(convert_event_content_open_tag(tag));
            mut_content.addItem(convert_event_content_open_tag_secondary(tag));
            parse_event_content_rec(tag, mut_content);
            mut_content.addItem(convert_event_content_closed_tag_secondary(tag));
            mut_content.addItem(convert_event_content_closed_tag(tag));
        } else if (node.isText()) {
            mut_content.addItem(convert_event_content_text(node.toText()));
        } else {
            invariant_violation("Did not expect node of type: %s", QP(xml_to_string(node)));
        }
    }
}

static CEDE(RawMessageContentItem) convert_event_content_open_tag(IMM(QDomElement) element) {
    QString tag_name = element.tagName();

    if (tag_name == "span") {
        return make_unique<CSSStyleTag>(read_only_string_attr(element, "style"));
    } else if (tag_name == "br") {
        return make_unique<LineBreak>();
    } else if (tag_name == "a") {
        return make_unique<LinkTag>(QUrl(read_string_attr(element, "href")));
    }

    invariant_violation("Unsupported open tag: <%s>", QP(tag_name));

    return unique_ptr<RawMessageContentItem>();
}

static CEDE(RawMessageContentItem) convert_event_content_closed_tag(IMM(QDomElement) element) {
    QString tag_name = element.tagName();

    if (tag_name == "span") {
        return make_unique<CSSStyleTag>();
    } else if (tag_name == "br") {
        return unique_ptr<RawMessageContentItem>();
    } else if (tag_name == "a") {
        return make_unique<LinkTag>(false);
    }

    invariant_violation("Unsupported closed tag: <%s>", QP(tag_name));

    return unique_ptr<RawMessageContentItem>();
}

static CEDE(RawMessageContentItem) convert_event_content_open_tag_secondary(IMM(QDomElement) element) {
    QString tag_name = element.tagName();

    if ((tag_name == "a") && (element.hasAttribute("style"))) {
        return make_unique<CSSStyleTag>(element.attribute("style"));
    }

    return unique_ptr<RawMessageContentItem>();
}

static CEDE(RawMessageContentItem) convert_event_content_closed_tag_secondary(IMM(QDomElement) element) {
    QString tag_name = element.tagName();

    if ((tag_name == "a") && (element.hasAttribute("style"))) {
        return make_unique<CSSStyleTag>();
    }

    return unique_ptr<RawMessageContentItem>();
}

static CEDE(TextSection) convert_event_content_text(IMM(QDomText) text_node) {
    QREGEX_WILL_MATCH(trim_match, "^\n*(.*)\n*$", text_node.nodeValue());

    QString trimmed_text = trim_match.captured(1);
    if (!trimmed_text.isEmpty()) {
        return make_unique<TextSection>(trimmed_text);
    }

    return unique_ptr<TextSection>();
}

}}}
