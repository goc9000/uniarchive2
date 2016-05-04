/**
 * extraction/facebook/extract_facebook_dyi_conversations.cpp
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

#include "extraction/facebook/extract_facebook_dyi_conversations.h"
#include "intermediate_format/content/TextSection.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "intermediate_format/subjects/SubjectGivenAsScreenName.h"
#include "intermediate_format/events/IFMessageEvent.h"
#include "protocols/facebook/account_name.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/language/invariant.h"
#include "utils/time/parse_date_parts.h"
#include "utils/xml/qdom_utils.h"

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols::facebook;
using namespace uniarchive2::utils::time;
using namespace uniarchive2::utils::xml;

namespace uniarchive2 { namespace extraction { namespace facebook {

IntermediateFormatConversation init_prototype(IMM(QString) filename);
QString read_identity_screen_name(IMM(QDomElement) root_element);
void extract_conversations_in_section(
    QDomElement& mut_next_element,
    vector<IntermediateFormatConversation>& mut_conversations,
    IMM(IntermediateFormatConversation) prototype
);
IntermediateFormatConversation extract_thread(
    QDomElement& mut_thread_element,
    IMM(IntermediateFormatConversation) prototype
);
void populate_thread_participants(
    IMM(QDomElement) thread_element,
    IntermediateFormatConversation& mut_conversation,
    IMM(IntermediateFormatConversation) prototype
);
CEDE(IntermediateFormatEvent) extract_message(
    QDomElement& mut_message_element
);
ApparentTime parse_message_time(IMM(QString) time_text);

vector<IntermediateFormatConversation> extract_facebook_dyi_conversations(IMM(QString) filename) {
    vector<IntermediateFormatConversation> conversations;
    IntermediateFormatConversation prototype = init_prototype(filename);

    QDomDocument xml = load_xml_file(filename);
    auto root_element = get_dom_root(xml, "html");

    QString identity_screen_name = read_identity_screen_name(root_element);
    prototype.identity = make_unique<SubjectGivenAsScreenName>(identity_screen_name);

    auto body_element = child_elem(root_element, "body");
    auto contents_div = only_child_elem_with_class(body_element, "div", "contents");

    auto element = contents_div.firstChildElement();
    invariant(element.tagName() == "h1", "Expected first element in contents div to be <h1>");

    element = element.nextSiblingElement();
    while (!element.isNull()) {
        extract_conversations_in_section(element, conversations, prototype);
    }

    return conversations;
}

IntermediateFormatConversation init_prototype(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", qUtf8Printable(filename));

    QString full_filename = file_info.absoluteFilePath();
    QString parent = full_filename.section(QDir::separator(), -2, -2);
    QString full_base_name = full_filename.section(QDir::separator(), -1, -1);

    invariant(
        (parent == "html") && (full_base_name == "messages.htm"),
        "Facebook DYI archive filename should have the form html/messages.htm, instead it looks like: %s",
        qUtf8Printable(filename.section(QDir::separator(), -2, -1))
    );

    IntermediateFormatConversation conversation(ArchiveFormats::FACEBOOK_DYI, IMProtocols::FACEBOOK);

    conversation.originalFilename = full_filename;
    conversation.fileLastModifiedTime = ApparentTime(
        file_info.lastModified().toTime_t(),
        ApparentTime::Reference::UNKNOWN
    );

    return conversation;
}

QString read_identity_screen_name(IMM(QDomElement) root_element) {
    auto head_element = child_elem(root_element, "head");
    auto title_element = child_elem(head_element, "title");

    QString title = read_text_only_content(title_element);

    static QRegularExpression title_pattern("^(.*) - Messages$");
    auto match = title_pattern.match(title);

    invariant(
        match.hasMatch(),
        "Expected title to be \"(screen name) - Messages\", but it is \"%s\"",
        qUtf8Printable(title)
    );

    return match.captured(1);
}

void extract_conversations_in_section(
    QDomElement& mut_next_element,
    vector<IntermediateFormatConversation>& mut_conversations,
    IMM(IntermediateFormatConversation) prototype
) {
    invariant(mut_next_element.tagName() == "div", "Expected section <div> to follow");

    auto thread_element = mut_next_element.firstChildElement();
    while (!thread_element.isNull()) {
        mut_conversations.push_back(extract_thread(thread_element, prototype));
        thread_element = thread_element.nextSiblingElement();
    }

    mut_next_element = mut_next_element.nextSiblingElement();
}

IntermediateFormatConversation extract_thread(
    QDomElement& mut_thread_element,
    IMM(IntermediateFormatConversation) prototype
) {
    invariant(
        (mut_thread_element.tagName() == "div") && (mut_thread_element.attribute("class", "") == "thread"),
        "Expected thread to be defined by a <div class=\"thread\">"
    );

    IntermediateFormatConversation conversation = IntermediateFormatConversation::fromPrototype(prototype);
    populate_thread_participants(mut_thread_element, conversation, prototype);

    vector<unique_ptr<IntermediateFormatEvent>> events_in_reverse;
    auto message_element = mut_thread_element.firstChildElement();
    while (!message_element.isNull()) {
        events_in_reverse.push_back(extract_message(message_element));
    }

    // Reverse and renumber messages
    move(events_in_reverse.rbegin(), events_in_reverse.rend(), back_inserter(conversation.events));
    unsigned int message_index = 0;
    for (auto& message : conversation.events) {
        message->indexInConversation = message_index++;
    }

    mut_thread_element = mut_thread_element.nextSiblingElement();

    return conversation;
}

void populate_thread_participants(
    IMM(QDomElement) thread_element,
    IntermediateFormatConversation& mut_conversation,
    IMM(IntermediateFormatConversation) prototype
) {
    auto participants_text_node = thread_element.firstChild();
    invariant(
        participants_text_node.isText(),
        "Expected thread node to start with text listing participants"
    );
    foreach (IMM(QString) name, participants_text_node.nodeValue().split(", ")) {
        if (is_valid_facebook_account_name(name)) {
            mut_conversation.declaredPeers.push_back(
                make_unique<SubjectGivenAsAccount>(parse_facebook_account(name))
            );
        } else if (name != static_cast<SubjectGivenAsScreenName*>(prototype.identity.get())->screenName) {
            mut_conversation.declaredPeers.push_back(make_unique<SubjectGivenAsScreenName>(name));
        }
    }
}

CEDE(IntermediateFormatEvent) extract_message(
    QDomElement& mut_message_element
) {
    invariant(
        (mut_message_element.tagName() == "div") && (mut_message_element.attribute("class", "") == "message"),
        "Expected message to be defined by a <div class=\"message\">"
    );

    auto header_element = only_child_elem_with_class(mut_message_element, "div", "message_header");

    auto user_element = only_child_elem_with_class(header_element, "span", "user");
    unique_ptr<ApparentSubject> sender = make_unique<SubjectGivenAsScreenName>(read_text_only_content(user_element));

    auto date_element = only_child_elem_with_class(header_element, "span", "meta");
    ApparentTime message_time = parse_message_time(read_text_only_content(date_element));

    IntermediateFormatMessageContent content;
    mut_message_element = mut_message_element.nextSiblingElement();
    invariant(mut_message_element.tagName() == "p", "Expeced a <p> to follow after the message <div>");

    QString message_text = read_text_only_content(mut_message_element);
    content.items.push_back(make_unique<TextSection>(message_text));

    mut_message_element = mut_message_element.nextSiblingElement();

    // Note: we fill in an index of 0 because the index can be computed only after we have all the messages (this is
    // because they are parsed in reverse)
    return make_unique<IFMessageEvent>(message_time, 0, move(sender), move(content));
}

ApparentTime parse_message_time(IMM(QString) time_text) {
    static QRegularExpression pattern("^\\w+, (\\w+) (\\d+), (\\d+) at (\\d+:\\d+(am|pm)) UTC(.*)$");

    auto match = pattern.match(time_text);
    invariant(match.hasMatch(), "Unexpected datetime format: \"%s\"", qUtf8Printable(time_text));

    int month = parse_english_month_name(match.captured(1));
    int day = match.captured(2).toInt();
    int year = match.captured(3).toInt();
    QDate date(year, month, day);
    invariant(date.isValid(), "Invalid date: \"%s\"", qUtf8Printable(time_text));

    QTime daytime = QTime::fromString(match.captured(4), "h:mma");
    invariant(daytime.isValid(), "Invalid time: \"%s\"", qUtf8Printable(match.captured(4)));

    int offset_quarters = parse_timezone_offset_in_quarters(match.captured(6));

    // The message date is absolute even if there is an offset (because it represents server time). Convert it to UTC
    // so that we don't have the illusion that we have local date info.
    QDateTime datetime(date, daytime, Qt::TimeSpec::OffsetFromUTC, 900 * offset_quarters);

    ApparentTime result(datetime.toUTC());
    result.secondsSpecified = false;

    return result;
}

}}}
