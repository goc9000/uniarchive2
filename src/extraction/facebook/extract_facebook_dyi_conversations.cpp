/**
 * extraction/facebook/extract_facebook_dyi_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "extraction/facebook/extract_facebook_dyi_conversations.h"
#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/provenance/ArchiveFileProvenance.h"
#include "intermediate_format/provenance/FileProvenance.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/subjects/AccountSubject.h"
#include "intermediate_format/subjects/ScreenNameSubject.h"
#include "intermediate_format/events/RawMessageEvent.h"
#include "protocols/facebook/facebook_account_name.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/time/parse_date_parts.h"
#include "utils/xml/qdom_utils.h"

#include <QtDebug>
#include <QLocale>

namespace uniarchive2 { namespace extraction { namespace facebook {

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::provenance;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols::facebook;
using namespace uniarchive2::utils::time;
using namespace uniarchive2::utils::xml;

static RawConversation init_prototype(IMM(AtomicConversationSource) source);
static QString read_identity_screen_name(IMM(QDomElement) root_element);
static void extract_conversations_in_section(
    IMM(QDomElement) section_element,
    vector<RawConversation>& mut_conversations,
    IMM(RawConversation) prototype
);
static RawConversation extract_thread(IMM(QDomElement) thread_element, IMM(RawConversation) prototype);
static void populate_thread_participants(
    IMM(QDomElement) thread_element,
    RawConversation& mut_conversation,
    IMM(RawConversation) prototype
);
static CEDE(RawEvent) extract_message(QDomElement& mut_message_element);
static ApparentTime parse_message_time(IMM(QString) time_text);


vector<RawConversation> extract_facebook_dyi_conversations(IMM(AtomicConversationSource) source) {
    vector<RawConversation> conversations;
    RawConversation prototype = init_prototype(source);

    QDomDocument xml = source.fullXML();
    auto root_element = get_dom_root(xml, "html");

    QString identity_screen_name = read_identity_screen_name(root_element);
    prototype.identity = make_unique<ScreenNameSubject>(identity_screen_name);

    auto body_element = child_elem(root_element, "body");
    auto contents_div = only_child_elem_with_class(body_element, "div", "contents");

    auto element = contents_div.firstChildElement();
    invariant(element.tagName() == "h1", "Expected first element in contents div to be <h1>");

    element = element.nextSiblingElement();
    while (!element.isNull()) {
        extract_conversations_in_section(element, conversations, prototype);
        element = element.nextSiblingElement();
    }

    return conversations;
}

static RawConversation init_prototype(IMM(AtomicConversationSource) source) {
    QString parent = source.logicalFilenameSection(-2);
    QString full_base_name = source.logicalFilename();

    invariant(
        (parent == "html") && (full_base_name == "messages.htm"),
        "Facebook DYI archive filename should have the form html/messages.htm, instead it looks like: %s",
        QP(source.logicalFilenameSections(-2, -1))
    );

    RawConversation conversation(IMProtocol::FACEBOOK);
    conversation.provenance = make_unique<ArchiveFileProvenance>(source.asProvenance(), ArchiveFormat::FACEBOOK_DYI);

    return conversation;
}

static QString read_identity_screen_name(IMM(QDomElement) root_element) {
    auto head_element = child_elem(root_element, "head");
    auto title_element = child_elem(head_element, "title");

    QString title = read_text_only_content(title_element);

    QREGEX_MUST_MATCH(
        match, "^(.*) - Messages$", title,
        "Expected title to be \"(screen name) - Messages\", but it is \"%s\""
    );

    return match.captured(1);
}

static void extract_conversations_in_section(
    IMM(QDomElement) section_element,
    vector<RawConversation>& mut_conversations,
    IMM(RawConversation) prototype
) {
    invariant(section_element.tagName() == "div", "Expected section <div> to follow");

    auto thread_element = section_element.firstChildElement();
    while (!thread_element.isNull()) {
        mut_conversations.push_back(extract_thread(thread_element, prototype));
        thread_element = thread_element.nextSiblingElement();
    }
}

static RawConversation extract_thread(IMM(QDomElement) thread_element, IMM(RawConversation) prototype) {
    invariant(
        (thread_element.tagName() == "div") && (thread_element.attribute("class", "") == "thread"),
        "Expected thread to be defined by a <div class=\"thread\">"
    );

    RawConversation conversation = RawConversation::fromPrototype(prototype);
    populate_thread_participants(thread_element, conversation, prototype);

    vector<unique_ptr<RawEvent>> events_in_reverse;
    auto message_element = thread_element.firstChildElement();
    while (!message_element.isNull()) {
        events_in_reverse.push_back(extract_message(message_element));
    }

    // Reverse and renumber messages
    move(events_in_reverse.rbegin(), events_in_reverse.rend(), back_inserter(conversation.events));
    uint32_t message_index = 0;
    for (auto& message : conversation.events) {
        message->indexInConversation = message_index++;
    }

    return conversation;
}

static void populate_thread_participants(
    IMM(QDomElement) thread_element,
    RawConversation& mut_conversation,
    IMM(RawConversation) prototype
) {
    auto participants_text_node = thread_element.firstChild();
    invariant(
        participants_text_node.isText(),
        "Expected thread node to start with text listing participants"
    );
    foreach (IMM(QString) name, participants_text_node.nodeValue().split(", ")) {
        if (is_valid_facebook_account_name(name)) {
            mut_conversation.declaredPeers.push_back(
                make_unique<AccountSubject>(parse_facebook_account(name))
            );
        } else if (name != prototype.identity->as<ScreenNameSubject>()->screenName) {
            mut_conversation.declaredPeers.push_back(make_unique<ScreenNameSubject>(name));
        }
    }
}

static CEDE(RawEvent) extract_message(QDomElement& mut_message_element) {
    invariant(
        (mut_message_element.tagName() == "div") && (mut_message_element.attribute("class", "") == "message"),
        "Expected message to be defined by a <div class=\"message\">"
    );

    auto header_element = only_child_elem_with_class(mut_message_element, "div", "message_header");

    auto user_element = only_child_elem_with_class(header_element, "span", "user");
    unique_ptr<ApparentSubject> sender = make_unique<ScreenNameSubject>(read_text_only_content(user_element));

    auto date_element = only_child_elem_with_class(header_element, "span", "meta");
    ApparentTime message_time = parse_message_time(read_text_only_content(date_element));

    mut_message_element = mut_message_element.nextSiblingElement();
    invariant(mut_message_element.tagName() == "p", "Expeced a <p> to follow after the message <div>");

    QString message_text = read_text_only_content(mut_message_element);
    mut_message_element = mut_message_element.nextSiblingElement();

    // Note: we fill in an index of 0 because the index can be computed only after we have all the messages (this is
    // because they are parsed in reverse)
    return make_unique<RawMessageEvent>(message_time, 0, move(sender), RawMessageContent::fromPlainText(message_text));
}

static ApparentTime parse_message_time(IMM(QString) time_text) {
    QREGEX_MUST_MATCH(
        match, "^\\w+, (\\w+) (\\d+), (\\d+) at (\\d+:\\d+(am|pm)) UTC(.*)$", time_text,
        "Unexpected datetime format: \"%s\""
    );

    int month = parse_english_month_name(match.captured(1));
    int day = match.captured(2).toInt();
    int year = match.captured(3).toInt();
    QDate date(year, month, day);
    invariant(date.isValid(), "Invalid date: \"%s\"", QP(time_text));

    QTime daytime = QLocale::c().toTime(match.captured(4), "h:mma");
    invariant(daytime.isValid(), "Invalid time: \"%s\"", QP(match.captured(4)));

    int offset_quarters = parse_timezone_offset_in_quarters(match.captured(6));

    // The message date is absolute even if there is an offset (because it represents server time). Convert it to UTC
    // so that we don't have the illusion that we have local date info.
    QDateTime datetime(date, daytime, Qt::TimeSpec::OffsetFromUTC, 900 * offset_quarters);

    ApparentTime result = ApparentTime::fromQDateTime(datetime.toUTC());
    result.secondsSpecified = false;

    return result;
}

}}}
