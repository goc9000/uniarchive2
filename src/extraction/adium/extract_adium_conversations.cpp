/**
 * extraction/adium/extract_adium_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <deque>

#include <QtDebug>
#include <QDir>
#include <QFileInfo>
#include <QIODevice>
#include <QMap>
#include <QRegularExpression>
#include <QUrl>

#include "extraction/adium/extract_adium_conversations.h"
#include "intermediate_format/content/CSSStyleTag.h"
#include "intermediate_format/content/LineBreakTag.h"
#include "intermediate_format/content/LinkTag.h"
#include "intermediate_format/content/TextSection.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/events/RawDisconnectedEvent.h"
#include "intermediate_format/events/RawCancelFileTransferEvent.h"
#include "intermediate_format/events/RawChangeScreenNameEvent.h"
#include "intermediate_format/events/RawConnectedEvent.h"
#include "intermediate_format/events/RawMessageEvent.h"
#include "intermediate_format/events/RawOfferFileEvent.h"
#include "intermediate_format/events/RawPingEvent.h"
#include "intermediate_format/events/RawReceiveFileEvent.h"
#include "intermediate_format/events/RawStartFileTransferEvent.h"
#include "intermediate_format/events/RawStatusChangeEvent.h"
#include "intermediate_format/events/RawUninterpretedEvent.h"
#include "intermediate_format/events/RawWindowClosedEvent.h"
#include "intermediate_format/events/RawWindowOpenedEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/subjects/FullySpecifiedSubject.h"
#include "intermediate_format/subjects/ImplicitSubject.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "intermediate_format/subjects/SubjectGivenAsScreenName.h"
#include "protocols/FullAccountName.h"
#include "protocols/IMProtocol.h"
#include "protocols/IMStatus.h"
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

RawConversation init_conversation(IMM(QString) filename);
InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename);
IMProtocol parse_protocol(IMM(QString) protocol_name);
void verify_identity(IMM(QDomElement) root_element, IMM(FullAccountName) identity);

CEDE(RawEvent) parse_event(IMM(QDomElement) event_element, IMM(RawConversation) conversation);
CEDE(ApparentSubject) parse_event_subject(IMM(QDomElement) event_element, IMM(RawConversation) conversation);
CEDE(RawEvent) parse_system_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    int event_index,
    TAKE(ApparentSubject) event_subject
);
CEDE(RawMessageEvent) parse_message_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    int event_index,
    TAKE(ApparentSubject) event_subject
);
CEDE(RawEvent) parse_status_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    int event_index,
    TAKE(ApparentSubject) event_subject
);
CEDE(RawStatusChangeEvent) parse_status_change_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    int event_index,
    TAKE(ApparentSubject) event_subject
);
CEDE(RawEvent) parse_purple_system_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    int event_index,
    TAKE(ApparentSubject) event_subject
);
RawMessageContent parse_event_content(IMM(QDomElement) event_element);
void parse_event_content_rec(IMM(QDomElement) element, RawMessageContent& mut_content);
CEDE(RawMessageContentItem) convert_event_content_open_tag(IMM(QDomElement) element);
CEDE(RawMessageContentItem) convert_event_content_closed_tag(IMM(QDomElement) element);
CEDE(RawMessageContentItem) convert_event_content_open_tag_secondary(IMM(QDomElement) element);
CEDE(RawMessageContentItem) convert_event_content_closed_tag_secondary(IMM(QDomElement) element);
CEDE(TextSection) convert_event_content_text(IMM(QDomText) text_node);


RawConversation extract_adium_conversation(IMM(QString) filename) {
    RawConversation conversation = init_conversation(filename);

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

RawConversation init_conversation(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", qUtf8Printable(filename));

    QString full_filename = file_info.absoluteFilePath();
    auto info = analyze_conversation_filename(full_filename);

    RawConversation conversation(ArchiveFormat::ADIUM, info.identity.protocol);

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

    IMProtocol protocol = parse_protocol(proto_and_id_match.captured(1));
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

IMProtocol parse_protocol(IMM(QString) protocol_name) {
    static QRegularExpression pattern("^((Yahoo!)|(Jabber))$");

    switch (pattern.match(protocol_name).lastCapturedIndex() - 2) {
        case 0: return IMProtocol::YAHOO;
        case 1: return IMProtocol::JABBER;
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

CEDE(RawEvent) parse_event(IMM(QDomElement) event_element, IMM(RawConversation) conversation) {
    ApparentTime event_time = ApparentTime(read_iso_date_attr(event_element, "time"));
    unique_ptr<ApparentSubject> event_subject = parse_event_subject(event_element, conversation);
    int event_index = (int)conversation.events.size();

    if (event_element.tagName() == "event") {
        return parse_system_event(event_element, event_time, event_index, move(event_subject));
    } else if (event_element.tagName() == "message") {
        return parse_message_event(event_element, event_time, event_index, move(event_subject));
    } else if (event_element.tagName() == "status") {
        return parse_status_event(event_element, event_time, event_index, move(event_subject));
    }

    invariant_violation("Unsupported event tag <%s>", qUtf8Printable(event_element.tagName()));
}

CEDE(ApparentSubject) parse_event_subject(IMM(QDomElement) event_element, IMM(RawConversation) conversation) {
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

CEDE(RawEvent) parse_system_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    int event_index,
    TAKE(ApparentSubject) event_subject
) {
    QString event_type = read_string_attr(event_element, "type");

    if (event_type == "windowOpened") {
        return make_unique<RawWindowOpenedEvent>(event_time, event_index);
    } else if (event_type == "windowClosed") {
        return make_unique<RawWindowClosedEvent>(event_time, event_index);
    }

    invariant_violation("Unsupported <event> type: %s", qUtf8Printable(event_type));
}

CEDE(RawMessageEvent) parse_message_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    int event_index,
    TAKE(ApparentSubject) event_subject
) {
    return make_unique<RawMessageEvent>(
        event_time,
        event_index,
        move(event_subject),
        move(parse_event_content(event_element))
    );
}

void expect_event_text(IMM(QDomElement) event_element, IMM(QString) expected_text) {
    QString event_text = event_element.text().trimmed();

    invariant(
        event_text == expected_text,
        "Expected event text to be \"%s\", found \"%s\"",
        qUtf8Printable(expected_text),
        qUtf8Printable(event_text)
    );
}

CEDE(RawEvent) parse_status_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    int event_index,
    TAKE(ApparentSubject) event_subject
) {
    QString event_type = event_element.attribute("type");

    if (EVENT_TYPE_TO_STATUS.contains(event_type)) {
        return parse_status_change_event(event_element, event_time, event_index, move(event_subject));
    } else if (event_type == "connected") {
        expect_event_text(event_element, "You have connected");
        return make_unique<RawConnectedEvent>(event_time, event_index);
    } else if (event_type == "disconnected") {
        expect_event_text(event_element, "You have disconnected");
        return make_unique<RawDisconnectedEvent>(event_time, event_index);
    } else if (event_type == "Notification") {
        if (event_element.text().trimmed().endsWith(" wants your attention!")) {
            return make_unique<RawPingEvent>(event_time, event_index, move(event_subject));
        }
        invariant_violation("Unsupported Notification event: %s", qUtf8Printable(xml_to_string(event_element)));
    } else if ((event_type == "purple") || (event_type == "libpurpleMessage")) {
        return parse_purple_system_event(event_element, event_time, event_index, move(event_subject));
    }

    invariant_violation("Unsupported <status> event type: %s", qUtf8Printable(event_type));
}

CEDE(RawStatusChangeEvent) parse_status_change_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    int event_index,
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

CEDE(RawEvent) parse_purple_system_event(
    IMM(QDomElement) event_element,
    ApparentTime event_time,
    int event_index,
    TAKE(ApparentSubject) event_subject
) {
    static QRegularExpression master_pattern(
        "^("
        "((?<rename_old>.+) is now known as (?<rename_new>.+)[.])|"
        "((?<offer_who>.+) is offering to send file (?<offer_file>.+))|"
        "((?<cancel_from>.+) cancelled the transfer of (?<cancel_file>.+))|"
        "(Starting transfer of (?<xfer_file>.+) from (?<xfer_from>.+))|"
        "(Transfer of file (?<recv_file>.+) complete)"
        ")$"
    );

    auto match = master_pattern.match(event_element.text().trimmed());

    if (match.capturedLength("rename_old")) {
        return make_unique<RawChangeScreenNameEvent>(
            event_time,
            event_index,
            make_unique<SubjectGivenAsScreenName>(match.captured("rename_old")),
            make_unique<SubjectGivenAsScreenName>(match.captured("rename_new"))
        );
    } else if (match.capturedLength("offer_who")) {
        return make_unique<RawOfferFileEvent>(
            event_time,
            event_index,
            make_unique<SubjectGivenAsScreenName>(match.captured("offer_who")),
            match.captured("offer_file")
        );
    } else if (match.capturedLength("xfer_file")) {
        unique_ptr<RawEvent> xfer_event =
            make_unique<RawStartFileTransferEvent>(event_time, event_index, match.captured("xfer_file"));
        static_cast<RawStartFileTransferEvent*>(xfer_event.get())->sender =
            make_unique<SubjectGivenAsScreenName>(match.captured("xfer_from"));

        return xfer_event;
    } else if (match.capturedLength("cancel_file")) {
        unique_ptr<RawEvent> xfer_event =
            make_unique<RawCancelFileTransferEvent>(event_time, event_index, match.captured("cancel_file"));
        static_cast<RawCancelFileTransferEvent*>(xfer_event.get())->sender =
            make_unique<SubjectGivenAsScreenName>(match.captured("cancel_from"));

        return xfer_event;
    } else if (match.capturedLength("recv_file")) {
        QString filename = match.captured("recv_file");

        deque<QDomElement> deq;
        deq.push_back(event_element);
        while (!deq.empty()) {
            auto elem = deq.front();
            if ((elem.tagName() == "a") && (elem.hasAttribute("href"))) {
                filename = elem.attribute("href");
                break;
            }
            deq.pop_front();
            for (
                auto child_elem = elem.firstChildElement();
                !child_elem.isNull();
                child_elem = child_elem.nextSiblingElement()
                ) {
                deq.push_back(child_elem);
            }
        }

        return make_unique<RawReceiveFileEvent>(
            event_time,
            event_index,
            make_unique<ImplicitSubject>(ImplicitSubject::Kind::FILE_RECEIVER),
            filename
        );
    }

    invariant_violation("Unsupported libpurple system message: %s", qUtf8Printable(xml_to_string(event_element)));
}

RawMessageContent parse_event_content(IMM(QDomElement) event_element) {
    RawMessageContent content;

    if (!event_element.hasChildNodes()) {
        return content;
    }

    auto main_div = only_child_elem(event_element, "div");
    invariant(main_div.attributes().length() == 0, "Did not expect content <div> to have attributes");

    parse_event_content_rec(main_div, content);

    return content;
}

void parse_event_content_rec(IMM(QDomElement) element, RawMessageContent& mut_content) {
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
            invariant_violation("Did not expect node of type: %s", qUtf8Printable(xml_to_string(node)));
        }
    }
}

CEDE(RawMessageContentItem) convert_event_content_open_tag(IMM(QDomElement) element) {
    QString tag_name = element.tagName();

    if (tag_name == "span") {
        return make_unique<CSSStyleTag>(read_only_string_attr(element, "style"));
    } else if (tag_name == "br") {
        return make_unique<LineBreakTag>();
    } else if (tag_name == "a") {
        return make_unique<LinkTag>(QUrl(read_string_attr(element, "href")));
    }

    invariant_violation("Unsupported open tag: <%s>", qUtf8Printable(tag_name));

    return unique_ptr<RawMessageContentItem>();
}

CEDE(RawMessageContentItem) convert_event_content_closed_tag(IMM(QDomElement) element) {
    QString tag_name = element.tagName();

    if (tag_name == "span") {
        return make_unique<CSSStyleTag>(true);
    } else if (tag_name == "br") {
        return unique_ptr<RawMessageContentItem>();
    } else if (tag_name == "a") {
        return make_unique<LinkTag>(true);
    }

    invariant_violation("Unsupported closed tag: <%s>", qUtf8Printable(tag_name));

    return unique_ptr<RawMessageContentItem>();
}

CEDE(RawMessageContentItem) convert_event_content_open_tag_secondary(IMM(QDomElement) element) {
    QString tag_name = element.tagName();

    if ((tag_name == "a") && (element.hasAttribute("style"))) {
        return make_unique<CSSStyleTag>(element.attribute("style"));
    }

    return unique_ptr<RawMessageContentItem>();
}

CEDE(RawMessageContentItem) convert_event_content_closed_tag_secondary(IMM(QDomElement) element) {
    QString tag_name = element.tagName();

    if ((tag_name == "a") && (element.hasAttribute("style"))) {
        return make_unique<CSSStyleTag>(true);
    }

    return unique_ptr<RawMessageContentItem>();
}

CEDE(TextSection) convert_event_content_text(IMM(QDomText) text_node) {
    static QRegularExpression pattern_trim("^\n*(.*)\n*$", QRegularExpression::DotMatchesEverythingOption);

    QString trimmed_text = pattern_trim.match(text_node.nodeValue()).captured(1);
    if (!trimmed_text.isEmpty()) {
        return make_unique<TextSection>(trimmed_text);
    }

    return unique_ptr<TextSection>();
}

}}}
