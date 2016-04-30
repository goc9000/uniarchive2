/**
 * extraction/msn/extract_msn_messenger_xml_conversations.cpp
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
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QRegularExpression>

#include "extraction/msn/extract_msn_messenger_xml_conversations.h"
#include "intermediate_format/content/CSSStyleTag.h"
#include "intermediate_format/content/TextSection.h"
#include "intermediate_format/events/IntermediateFormatEvent.h"
#include "intermediate_format/events/IFOfferFileEvent.h"
#include "intermediate_format/events/IFReceiveFileEvent.h"
#include "intermediate_format/events/IFOfferCallEvent.h"
#include "intermediate_format/events/IFAnswerCallEvent.h"
#include "intermediate_format/events/IFMessageEvent.h"
#include "intermediate_format/subjects/ImplicitSubject.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "intermediate_format/subjects/SubjectGivenAsScreenName.h"
#include "protocols/msn/account_name.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/language/invariant.h"
#include "utils/xml/qdom_utils.h"

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols::msn;
using namespace uniarchive2::utils::xml;

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
    const ApparentTime& event_time,
    unsigned int event_index,
    bool is_response
);
unique_ptr<IntermediateFormatEvent> parse_invitation_or_response_event_with_file(
    const QDomElement& event_element,
    const ApparentTime& event_time,
    unsigned int event_index,
    bool is_response,
    unique_ptr<ApparentSubject> subject,
    const QString& text,
    const QString& filename
);
unique_ptr<IntermediateFormatEvent> parse_invitation_or_response_event_with_application(
    const QDomElement& event_element,
    const ApparentTime& event_time,
    unsigned int event_index,
    bool is_response,
    unique_ptr<ApparentSubject> subject,
    const QString& text,
    const QString& application
);
ApparentTime parse_event_time(const QDomElement& event_element);
unique_ptr<ApparentSubject> parse_event_actor(const QDomElement& event_element, const QString& node_name);
IntermediateFormatMessageContent parse_event_text(const QDomElement& event_element);

vector<IntermediateFormatConversation> extract_msn_messenger_xml_conversations(const QString &filename) {
    vector<IntermediateFormatConversation> conversations;
    IntermediateFormatConversation prototype = init_prototype(filename);

    QDomDocument xml = load_xml_file(filename);
    QDomElement root_element = get_dom_root(xml, "Log");

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
    if (remote_account.accountName != local_account.accountName) {
        conversation.declaredPeers.push_back(make_unique<SubjectGivenAsAccount>(remote_account));
    }

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
        return parse_invitation_or_response_event(event_element, event_time, event_index, false);
    } else if (event_element.tagName() == "InvitationResponse") {
        return parse_invitation_or_response_event(event_element, event_time, event_index, true);
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
    if (text_element.hasAttribute("Style")) {
        content.items.push_back(make_unique<CSSStyleTag>(read_string_attr(text_element, "Style")));
    }

    content.items.push_back(make_unique<TextSection>(read_text_only_content(text_element)));

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
    const ApparentTime& event_time,
    unsigned int event_index,
    bool is_response
) {
    unique_ptr<ApparentSubject> subject = parse_event_actor(event_element, "From");
    QString text = read_text_only_content(child_elem(event_element, "Text"));

    QDomElement file_element = event_element.firstChildElement("File");
    if (!file_element.isNull()) {
        QString filename = read_text_only_content(file_element);
        return parse_invitation_or_response_event_with_file(
            event_element, event_time, event_index, is_response, move(subject), text, filename
        );
    }
    QDomElement app_element = event_element.firstChildElement("Application");
    if (!app_element.isNull()) {
        QString application = read_text_only_content(app_element);
        return parse_invitation_or_response_event_with_application(
            event_element, event_time, event_index, is_response, move(subject), text, application
        );
    }

    invariant_violation("Unhandled invitation event: %s", qUtf8Printable(xml_to_string(event_element)));
}

unique_ptr<IntermediateFormatEvent> parse_invitation_or_response_event_with_file(
    const QDomElement& event_element,
    const ApparentTime& event_time,
    unsigned int event_index,
    bool is_response,
    unique_ptr<ApparentSubject> subject,
    const QString& text,
    const QString& filename
) {
    static QRegularExpression pat_transfer_complete("^Transfer of .* is complete[.]$");

    if (!is_response) {
        if (text.contains(" sends ")) {
            return make_unique<IFOfferFileEvent>(event_time, event_index, move(subject), filename);
        }
    } else {
        if (text.contains("You have successfully received ")) {
            unique_ptr<IntermediateFormatEvent> event = make_unique<IFReceiveFileEvent>(
                event_time,
                event_index,
                make_unique<ImplicitSubject>(ImplicitSubject::Kind::IDENTITY),
                filename
            );
            static_cast<IFReceiveFileEvent*>(event.get())->sender = move(subject);

            return event;
        } else if (pat_transfer_complete.match(text).hasMatch()) {
            return make_unique<IFReceiveFileEvent>(event_time, event_index, move(subject), filename);
        }
    }

    invariant_violation("Unhandled file transfer event: %s", qUtf8Printable(xml_to_string(event_element)));
}

unique_ptr<IntermediateFormatEvent> parse_invitation_or_response_event_with_application(
    const QDomElement& event_element,
    const ApparentTime& event_time,
    unsigned int event_index,
    bool is_response,
    unique_ptr<ApparentSubject> subject,
    const QString& text,
    const QString& application
) {
    if (!is_response) {
        if (text.contains(" is calling you") && (application == "a Computer Call")) {
            return make_unique<IFOfferCallEvent>(event_time, event_index, move(subject));
        }
    } else {
        if (text.contains("You have answered") && (application == "a Computer Call")) {
            unique_ptr<IntermediateFormatEvent> event = make_unique<IFAnswerCallEvent>(
                event_time,
                event_index,
                make_unique<ImplicitSubject>(ImplicitSubject::Kind::IDENTITY)
            );
            static_cast<IFAnswerCallEvent*>(event.get())->caller = move(subject);

            return event;
        }
    }

    invariant_violation("Unhandled application invite event: %s", qUtf8Printable(xml_to_string(event_element)));
}

}}}
