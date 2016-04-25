/**
 * extraction/yahoo/extract_msn_messenger_xml_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <cmath>

#include <QtDebug>
#include <QDateTime>
#include <QDir>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>

#include "extraction/msn/extract_msn_messenger_xml_conversations.h"
#include "intermediate_format/content/CSSStyleTag.h"
#include "intermediate_format/content/TextSection.h"
#include "intermediate_format/events/IntermediateFormatEvent.h"
#include "intermediate_format/events/IFMessageEvent.h"
#include "intermediate_format/events/IFUninterpretedEvent.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "intermediate_format/subjects/SubjectGivenAsScreenName.h"
#include "protocols/msn/account_name.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/language/invariant.h"

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols::msn;

namespace uniarchive2 { namespace extraction { namespace msn {

IntermediateFormatConversation init_prototype(const QString& filename);
IntermediateFormatConversation extract_conversation_for_session(
    QDomElement& mut_next_element,
    int session_id,
    const IntermediateFormatConversation& prototype,
    unsigned int conversation_no_in_file,
    unsigned int conversation_offset_in_file
);
unique_ptr<IntermediateFormatEvent> parse_event(const QDomElement& event_element, unsigned int event_index);
unique_ptr<IntermediateFormatEvent> parse_message_event(
    const QDomElement& event_element,
    const ApparentTime& event_time,
    unsigned int event_index
);
unique_ptr<IntermediateFormatEvent> parse_invitation_or_response_event(
    const QDomElement& event_element,
    bool is_response,
    const ApparentTime& event_time,
    unsigned int event_index
);
ApparentTime parse_event_time(const QDomElement& event_element);
unique_ptr<ApparentSubject> parse_event_actor(const QDomElement& event_element, const QString& node_name);
IntermediateFormatMessageContent parse_event_text(const QDomElement& event_element);

QDomDocument load_xml_file(const QString& filename);
QDomElement child_elem(const QDomElement& node, const QString& child_name);
QDomElement only_child_elem(const QDomElement& node, const QString& child_name);
int read_int_attr(const QDomElement& node, const QString& attr_name);
QString read_string_attr(const QDomElement& node, const QString& attr_name);
QDateTime read_iso_date_attr(const QDomElement& node, const QString& attr_name);
QString xml_to_string(const QDomNode& node);
QByteArray xml_to_raw_data(const QDomNode& node);

vector<IntermediateFormatConversation> extract_msn_messenger_xml_conversations(const QString &filename) {
    vector<IntermediateFormatConversation> conversations;
    IntermediateFormatConversation prototype = init_prototype(filename);

    QDomDocument xml = load_xml_file(filename);
    QDomElement root_element = xml.documentElement();
    invariant(root_element.tagName() == "Log", "MSN archive root node should be <Log>");

    int first_session_id = read_int_attr(root_element, "FirstSessionID");
    int last_session_id = read_int_attr(root_element, "LastSessionID");
    invariant(first_session_id < last_session_id, "FirstSessionID should be <= LastSessionID");

    QDomElement next_element = root_element.firstChildElement();
    int expected_session_id = first_session_id;
    unsigned int message_offset = 0;

    while (!next_element.isNull()) {
        conversations.push_back(
            extract_conversation_for_session(
                next_element,
                expected_session_id,
                prototype,
                (unsigned int)(conversations.size() + 1),
                message_offset
            )
        );
        expected_session_id++;
    }

    return conversations;
}

IntermediateFormatConversation init_prototype(const QString& filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", qUtf8Printable(filename));

    QString full_filename = file_info.absoluteFilePath();
    QString grand_parent = full_filename.section(QDir::separator(), -3, -3);
    QString parent = full_filename.section(QDir::separator(), -2, -2);
    QString base_name = file_info.completeBaseName();

    invariant(
        (
            is_likely_valid_optionally_encoded_msn_account_name(grand_parent) &&
            (parent == "History") &&
            is_likely_valid_optionally_encoded_msn_account_name(base_name) &&
            (file_info.suffix().toLower() == "xml")
        ),
        "MSN archive filename should have the form <local account name>/History/<remote account name>.xml, instead "
            "it looks like: %s",
        qUtf8Printable(filename.section(QDir::separator(), -3, -1))
    );

    IntermediateFormatConversation conversation(ArchiveFormats::MSN_MESSENGER_XML, IMProtocols::MSN);

    conversation.originalFilename = full_filename;
    conversation.fileLastModifiedTime = ApparentTime(
        file_info.lastModified().toTime_t(),
        ApparentTime::Reference::UNKNOWN
    );

    auto local_account = parse_optionally_encoded_msn_account(grand_parent);
    auto remote_account = parse_optionally_encoded_msn_account(base_name);

    conversation.identity = make_unique<SubjectGivenAsAccount>(local_account);
    conversation.declaredPeers.push_back(make_unique<SubjectGivenAsAccount>(remote_account));

    return conversation;
}

IntermediateFormatConversation extract_conversation_for_session(
    QDomElement& mut_next_element,
    int expected_session_id,
    const IntermediateFormatConversation& prototype,
    unsigned int conversation_no_in_file,
    unsigned int conversation_offset_in_file
) {
    invariant(!mut_next_element.isNull(), "Attempted to read session when none are left");

    int session_id = read_int_attr(mut_next_element, "SessionID");
    invariant(
        session_id == expected_session_id, "Expected session ID = %d, found %d",  expected_session_id, session_id
    );

    auto conversation = IntermediateFormatConversation::fromPrototype(prototype);
    conversation.numConversationInFile = conversation_no_in_file;
    conversation.conversationOffsetInFileEventBased = conversation_offset_in_file;

    while (!mut_next_element.isNull() && (read_int_attr(mut_next_element, "SessionID") == session_id)) {
        conversation.events.push_back(parse_event(mut_next_element, (unsigned int)conversation.events.size()));
        mut_next_element = mut_next_element.nextSiblingElement();
    }

    return conversation;
}

unique_ptr<IntermediateFormatEvent> parse_event(const QDomElement& event_element, unsigned int event_index) {
    ApparentTime event_time = parse_event_time(event_element);

    if (event_element.tagName() == "Message") {
        return parse_message_event(event_element, event_time, event_index);
    } else if (event_element.tagName() == "Invitation") {
        return parse_invitation_or_response_event(event_element, false, event_time, event_index);
    } else if (event_element.tagName() == "InvitationResponse") {
        return parse_invitation_or_response_event(event_element, true, event_time, event_index);
    }

    invariant_violation("Can't handle MSN event node of type %s", qUtf8Printable(event_element.tagName()));
}

ApparentTime parse_event_time(const QDomElement& event_element) {
    QDateTime absolute_time = read_iso_date_attr(event_element, "DateTime");
    invariant(absolute_time.timeSpec() == Qt::UTC, "Expected MSN message datetime to be UTC");

    QString local_time_str =
        read_string_attr(event_element, "Date").append(" ").append(read_string_attr(event_element, "Time"));

    QDateTime local_time = QDateTime::fromString(local_time_str, "dd/MM/yyyy HH:mm:ss");
    invariant(local_time.isValid(), "Failed to parse local Date/Time");

    local_time.setTimeSpec(Qt::UTC);

    qint64 offsetMsecs = absolute_time.msecsTo(local_time);
    int offsetQuarters = (int)lround(offsetMsecs / 900000.0);
    qint64 difference = abs(offsetQuarters * 900000 - (int)offsetMsecs);

    invariant(abs(offsetQuarters) < 14*4, "Local and absolute time too far apart (>14h)");
    invariant(difference < 2000, "Local and absolute time not offset by a whole number of quarter-hours");

    absolute_time = absolute_time.toOffsetFromUtc(900 * offsetQuarters);

    return ApparentTime(absolute_time);
}

unique_ptr<ApparentSubject> parse_event_actor(const QDomElement& event_element, const QString& node_name) {
    auto actor_element = child_elem(event_element, node_name);
    auto user_element = only_child_elem(actor_element, "User");

    return make_unique<SubjectGivenAsScreenName>(read_string_attr(user_element, "FriendlyName"));
}

IntermediateFormatMessageContent parse_event_text(const QDomElement& event_element) {
    IntermediateFormatMessageContent content;

    auto text_element = child_elem(event_element, "Text");
    invariant(text_element.firstChildElement().isNull(), "Not expecting <Text> node to have sub-elements");

    if (text_element.hasAttribute("Style")) {
        content.items.push_back(make_unique<CSSStyleTag>(read_string_attr(text_element, "Style")));
    }
    content.items.push_back(make_unique<TextSection>(text_element.text()));

    return content;
}

unique_ptr<IntermediateFormatEvent> parse_message_event(
    const QDomElement& event_element,
    const ApparentTime& event_time,
    unsigned int event_index
) {
    unique_ptr<IntermediateFormatEvent> event = make_unique<IFMessageEvent>(
        event_time,
        event_index,
        parse_event_actor(event_element, "From"),
        parse_event_text(event_element)
    );
    static_cast<IFMessageEvent*>(event.get())->receiver = parse_event_actor(event_element, "To");

    return event;
}

unique_ptr<IntermediateFormatEvent> parse_invitation_or_response_event(
    const QDomElement& event_element,
    bool is_response,
    const ApparentTime& event_time,
    unsigned int event_index
) {
    return make_unique<IFUninterpretedEvent>(event_time, event_index, xml_to_raw_data(event_element));
}

QDomDocument load_xml_file(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal("Can't open file: %s", qUtf8Printable(filename));
    }

    QDomDocument xml;
    QString error_message;
    int error_line, error_column;

    if (!xml.setContent(&file, false, &error_message, &error_line, &error_column)) {
        qFatal(
            "Error reading XML file '%s': %s (at line %d, column %d)",
            qUtf8Printable(filename),
            qUtf8Printable(error_message),
            error_line,
            error_column
        );
    }

    return xml;
}

QDomElement child_elem(const QDomElement& node, const QString& child_name) {
    QDomElement child_node = node.firstChildElement(child_name);
    invariant(
        !child_node.isNull(),
        "Expected to find <%s> node under <%s>",
        qUtf8Printable(child_name),
        qUtf8Printable(node.tagName())
    );

    return child_node;
}

QDomElement only_child_elem(const QDomElement& node, const QString& child_name) {
    QDomElement child_node = node.firstChildElement();
    invariant(
        (child_node.tagName() == child_name) && child_node.nextSiblingElement().isNull(),
        "Expected to find only a <%s> node under <%s>",
        qUtf8Printable(child_name),
        qUtf8Printable(node.tagName())
    );

    return child_node;
}

int read_int_attr(const QDomElement& node, const QString& attr_name) {
    QString value_text = read_string_attr(node, attr_name);
    bool ok = false;
    int value = value_text.toInt(&ok);

    invariant(ok, "Invalid integer attribute value: '%s'", qUtf8Printable(value_text));

    return value;
}

QString read_string_attr(const QDomElement& node, const QString& attr_name) {
    invariant(node.hasAttribute(attr_name), "Node is missing attribute '%s'", qUtf8Printable(attr_name));

    return node.attribute(attr_name);
}

QDateTime read_iso_date_attr(const QDomElement& node, const QString& attr_name) {
    QString value_text = read_string_attr(node, attr_name);

    QDateTime value = QDateTime::fromString(value_text, Qt::ISODate);
    invariant(value.isValid(), "Invalid ISO datetime attribute value: '%s'", qUtf8Printable(value_text));

    return value;
}

QString xml_to_string(const QDomNode& node) {
    QString text;
    QTextStream stream(&text);

    node.save(stream, 0);

    return text;
}

QByteArray xml_to_raw_data(const QDomNode& node) {
    QByteArray raw_data;
    QTextStream stream(&raw_data);

    node.save(stream, 0);

    return raw_data;
}

}}}
