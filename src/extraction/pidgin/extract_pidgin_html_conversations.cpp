/**
 * extraction/pidgin/extract_pidgin_html_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>
#include <QFileInfo>
#include <QIODevice>
#include <QTextCodec>
#include <QTimeZone>

#include "extraction/pidgin/extract_pidgin_html_conversations.h"
#include "extraction/pidgin/common_extract_pidgin_conversations_code.h"
#include "extraction/parse_libpurple_system_message.h"
#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/content/CSSStyleTag.h"
#include "intermediate_format/content/EmphasisTag.h"
#include "intermediate_format/content/LineBreakTag.h"
#include "intermediate_format/content/LinkTag.h"
#include "intermediate_format/content/TextSection.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/events/RawMessageEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "intermediate_format/subjects/SubjectGivenAsScreenName.h"
#include "protocols/FullAccountName.h"
#include "protocols/IMProtocol.h"
#include "protocols/IMStatus.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/external_libs/optional.hpp"
#include "utils/html/entities.h"
#include "utils/html/parse_html_lenient.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/time/parse_date_parts.h"

using namespace std;
using namespace uniarchive2::extraction;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;
using namespace uniarchive2::utils::html;
using namespace uniarchive2::utils::time;

namespace uniarchive2 { namespace extraction { namespace pidgin {

static void verify_is_utf8_html(QTextStream& mut_stream);
static void seek_to_start_of_events(QTextStream& mut_stream);

static CEDE(RawEvent) parse_event(IMM(QRegularExpressionMatch) event_match, IMM(RawConversation) conversation);
static CEDE(RawMessageEvent) parse_message(
    unsigned int index, IMM(QString) color, IMM(ApparentTime) timestamp, IMM(QString) sender, IMM(QString) message_html
);
static QString strip_sender_suffix(IMM(QString) sender);
static RawMessageContent parse_message_content(IMM(QString) content_html);
static CEDE(RawMessageContentItem) parse_markup_tag(IMM(ParsedHTMLTagInfo) tag_info);
static CEDE(TextSection) parse_text_section(IMM(QString) text);


RawConversation extract_pidgin_html_conversation(IMM(QString) filename) {
    RawConversation conversation = init_conversation(filename, "html", ArchiveFormat::PIDGIN_HTML);

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal("Can't open file: %s", QP(filename));
    }

    // Use regular expressions because newlines are very significant in parsing here
    QTextStream stream(&file);
    verify_is_utf8_html(stream);
    seek_to_start_of_events(stream);

    QString full_text = stream.readAll();

    QREGEX_CI(
        event_pattern,
        "("\
          "(<font color=\"(?<color>#[a-z0-9]{6})\">)?" \
          "<font size=\"2\">\\((?<timestamp>[^)]*)\\)</font>" \
          "(" \
            "( <b>(?<sender>[^:]*):</b></font> (?<message>.*?))|" \
            "(<b> (?<system_message>.*?)</b>(</font>)?)" \
          ")" \
          "<br/>\r?\n" \
        ")|("\
          "?<end_of_file></body></html>\\s*$"\
        ")"
    );

    int offset = 0;
    while (offset < full_text.length()) {
        auto event_match = event_pattern.match(full_text, offset);
        invariant(
            event_match.hasMatch() && (event_match.capturedStart(0) == offset),
            "Failed to parse event at text: \"%s...\"", QP(full_text.mid(offset, 256))
        );
        if (event_match.capturedLength("end_of_file")) {
            break;
        }

        conversation.events.push_back(parse_event(event_match, conversation));

        offset = event_match.capturedEnd(0);
    }

    return conversation;
}

static void verify_is_utf8_html(QTextStream& mut_stream) {
    QByteArray expected_header("<html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">");
    QByteArray actual_header = mut_stream.device()->peek(expected_header.length());

    invariant(
         actual_header == expected_header,
         "Expected header to be \"\", found \"\"", qPrintable(expected_header), qPrintable(actual_header)
    );

    mut_stream.setCodec(QTextCodec::codecForName("UTF-8"));
}

static void seek_to_start_of_events(QTextStream& mut_stream) {
    QString first_line = mut_stream.readLine();

    QREGEX_MUST_MATCH_CI(
        match,
        "^<html><head><meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">"\
        "<title>Conversation with (.*) at (.*) on (.*) \\((.*)\\)</title>"\
        "</head><body><h3>Conversation with (.*) at (.*) on (.*) \\((.*)\\)</h3>$",
        first_line,
        "Unexpected format for first line of archive: \"%s\""
    );
}

static CEDE(RawEvent) parse_event(IMM(QRegularExpressionMatch) event_match, IMM(RawConversation) conversation) {
    QString color = event_match.captured("color");
    ApparentTime timestamp = parse_timestamp(event_match.captured("timestamp"), conversation);

    if (!event_match.capturedLength("system_message")) {
        return parse_message(
            (unsigned int)conversation.events.size(),
            color,
            timestamp,
            decode_html_entities(event_match.captured("sender")),
            event_match.captured("message")
        );
    } else {
        invariant(color == "" || color == "#FF0000", "Expected color to be absent or #FF0000 for system message");
        return parse_libpurple_system_message(
            (unsigned int)conversation.events.size(),
            timestamp,
            event_match.captured("system_message"),
            conversation.protocol
        );
    }
}

static CEDE(RawMessageEvent) parse_message(
    unsigned int index, IMM(QString) color, IMM(ApparentTime) timestamp, IMM(QString) sender, IMM(QString) message_html
) {
    invariant(
        color == "#16569E" || color == "#A82F2F",
        "Expected color to be #16569E or #A82F2F for system message"
    );

    return make_unique<RawMessageEvent>(
        timestamp,
        index,
        make_unique<SubjectGivenAsScreenName>(
            strip_sender_suffix(sender),
            (color == "#16569E") ? ApparentSubject::Hint::IsIdentity : ApparentSubject::Hint::IsPeer
        ),
        parse_message_content(message_html)
    );
}

static QString strip_sender_suffix(IMM(QString) sender) {
    QREGEX_MATCH_CI(match, "(.*@[^/]*)/.*", sender);
    return match.hasMatch() ? match.captured(1) : sender;
}

static RawMessageContent parse_message_content(IMM(QString) content_html) {
    RawMessageContent content;

    auto lenient_parse_result = parse_html_lenient(content_html);

    for (int i = 0; i < lenient_parse_result.textSections.size(); i++) {
        if (i > 0) {
            content.addItem(parse_markup_tag(lenient_parse_result.tags[i-1]));
        }
        content.addItem(parse_text_section(lenient_parse_result.textSections[i]));
    }

    return content;
}

static CEDE(RawMessageContentItem) parse_markup_tag(IMM(ParsedHTMLTagInfo) tag_info) {
    if (tag_info.tagName == "br") {
        return make_unique<LineBreakTag>();
    }

    if (tag_info.open && tag_info.closed) {
        if (tag_info.tagName == "span") {
            return unique_ptr<RawMessageContentItem>(); // skip these tags if self-closed
        }
        invariant_violation("Did not expect this self-closing tag in a Pidign archive: <%s/>", QP(tag_info.tagName));
    }

    if ((tag_info.tagName == "html") || (tag_info.tagName == "body") || (tag_info.tagName == "p")) {
        return unique_ptr<RawMessageContentItem>(); // skip these tags
    } else if (tag_info.tagName == "em") {
        return make_unique<EmphasisTag>(tag_info.closed);
    } else if (tag_info.tagName == "span") {
        if (tag_info.closed) {
            return make_unique<CSSStyleTag>(true);
        } else {
            invariant(
                tag_info.attributes.empty() ||
                ((tag_info.attributes.size() == 1) && (tag_info.attributes.keys().first() == "style")),
                "Expected <span style=\"(css)\"> to be the only use of the SPAN tag"
            );

            QString style = tag_info.attributes["style"];

            // HAXX: sometimes this attribute is double-encoded, try to fix it
            style = decode_html_entities(style);

            return make_unique<CSSStyleTag>(style);
        }
    } else if (tag_info.tagName == "a") {
        if (tag_info.closed) {
            return make_unique<LinkTag>(true);
        } else {
            return make_unique<LinkTag>(QUrl(tag_info.attributes["href"]));
        }
    }

    // If the tag is not recognized, return it as unparsed text
    return make_unique<TextSection>(tag_info.originalText);
}

static CEDE(TextSection) parse_text_section(IMM(QString) text) {
    if (text.isEmpty()) {
        return unique_ptr<TextSection>();
    }

    return make_unique<TextSection>(text);
}

}}}