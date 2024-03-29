/**
 * extraction/yahoo/extract_yahoo_messenger_dat_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "extraction/yahoo/extract_yahoo_messenger_dat_conversations.h"
#include "extraction/yahoo/ExtractYahooProtocolEventsIterator.h"
#include "graphics/Color.h"
#include "graphics/ANSIColor.h"
#include "intermediate_format/ApparentTime.h"
#include "intermediate_format/content/formatting/ANSIColorTag.h"
#include "intermediate_format/content/formatting/ANSIResetTag.h"
#include "intermediate_format/content/formatting/BoldTag.h"
#include "intermediate_format/content/formatting/ItalicTag.h"
#include "intermediate_format/content/formatting/UnderlineTag.h"
#include "intermediate_format/content/formatting/LinkTag.h"
#include "intermediate_format/content/formatting/RGBColorTag.h"
#include "intermediate_format/content/formatting/FontTag.h"
#include "intermediate_format/content/formatting/YahooAltTag.h"
#include "intermediate_format/content/formatting/YahooFadeTag.h"
#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/content/TextSection.h"
#include "intermediate_format/events/conference/RawJoinConferenceEvent.h"
#include "intermediate_format/events/conference/RawDeclineConferenceEvent.h"
#include "intermediate_format/events/conference/RawLeaveConferenceEvent.h"
#include "intermediate_format/events/conversation/RawStartConversationEvent.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/events/RawMessageEvent.h"
#include "intermediate_format/provenance/ArchiveFileProvenance.h"
#include "intermediate_format/provenance/FileProvenance.h"
#include "intermediate_format/provenance/EventRangeProvenance.h"
#include "intermediate_format/subjects/AccountSubject.h"
#include "protocols/yahoo/yahoo_account_name.h"
#include "protocols/ArchiveFormat.h"
#include "protocols/FullAccountName.h"
#include "utils/language/invariant.h"
#include "utils/html/parse_html_lenient.h"
#include "utils/qt/shortcuts.h"
#include "utils/text/decoding.h"

#include <QtDebug>

namespace uniarchive2 { namespace extraction { namespace yahoo {

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::provenance;
using namespace uniarchive2::protocols::yahoo;
using namespace uniarchive2::utils::html;
using namespace uniarchive2::utils::text;

static RawConversation init_prototype(IMM(AtomicConversationSource) source);
static void flush_conversation(
    vector<RawConversation>& mut_conversations,
    IMM(RawConversation) prototype,
    vector<unique_ptr<RawEvent>>&& mut_current_convo_events,
    uint32_t start_event_index,
    uint32_t end_event_index
);
static CEDE(RawEvent) convert_event(
    IMM(YahooProtocolEvent) proto_event,
    uint32_t event_index,
    IMM(RawConversation) prototype
);
static CEDE(ApparentSubject) implicit_subject(IMM(YahooProtocolEvent) proto_event, IMM(RawConversation) conversation);
static CEDE(ApparentSubject) parse_event_subject(
    IMM(YahooProtocolEvent) proto_event,
    IMM(RawConversation) conversation
);
static RawMessageContent parse_message_content(IMM(QByteArray) text_data);
static CEDE(TextSection) make_text_section(IMM(QString) text);
static CEDE(RawMessageContentItem) parse_markup_tag(IMM(QRegularExpressionMatch) match);
static CEDE(RawMessageContentItem) parse_pseudo_ansi_seq(IMM(QString) sgr_code);
static CEDE(RawMessageContentItem) parse_html_tag(IMM(QString) tag_text);
static CEDE(FontTag) parse_font_tag(IMM(ParsedHTMLTagInfo));
static CEDE(RawMessageContentItem) parse_yahoo_tag(IMM(QString) tag_text);


vector<RawConversation> extract_yahoo_messenger_dat_conversations(IMM(AtomicConversationSource) source) {
    vector<RawConversation> conversations;
    RawConversation prototype = init_prototype(source);

    QString local_account_name = prototype.identity->as<AccountSubject>()->account.accountName;
    ExtractYahooProtocolEventsIterator proto_events(source.fullData(), local_account_name);

    vector<unique_ptr<RawEvent>> current_events;
    uint32_t first_event_index = 0;
    uint32_t event_index = 0;

    while (proto_events.hasNext()) {
        auto proto_event = proto_events.next();
        if ((proto_event.type == YahooProtocolEvent::Type::START_CONVERSATION) && (event_index > 0)) {
            flush_conversation(conversations, prototype, move(current_events), first_event_index, event_index - 1);
            first_event_index = event_index;
        }

        current_events.push_back(convert_event(proto_event, current_events.size(), prototype));
        event_index++;
    }

    if (event_index > 0) {
        flush_conversation(conversations, prototype, move(current_events), first_event_index, event_index - 1);
    }

    return conversations;
}

static RawConversation init_prototype(IMM(AtomicConversationSource) source) {
    QREGEX_MUST_MATCH_CI(
        match, "^\\d{8}-(.+)[.]dat$", source.logicalFilename(),
        "Yahoo archive filename does not have the form \"YYYYMMDD-account_name.dat\", it is \"%s\""
    );
    auto local_account = parse_yahoo_account(match.captured(1));

    RawConversation conversation(IMProtocol::YAHOO);
    conversation.provenance =
        make_unique<ArchiveFileProvenance>(source.asProvenance(), ArchiveFormat::YAHOO_MESSENGER_DAT);

    conversation.identity = make_unique<AccountSubject>(local_account);
    auto remote_account = parse_yahoo_account(source.logicalFilenameSection(-2));
    conversation.declaredPeers.push_back(make_unique<AccountSubject>(remote_account));

    QString top_folder = source.logicalFilenameSection(-3);
    if (top_folder == "Messages") {
        conversation.isConference = false;
    } else if (top_folder == "Conferences") {
        conversation.isConference = true;
    } else {
        invariant_violation("Yahoo Messenger DAT archive file must be in a 'Messages' or 'Conferences' folder");
    }

    return conversation;
}

static void flush_conversation(
    vector<RawConversation>& mut_conversations,
    IMM(RawConversation) prototype,
    vector<unique_ptr<RawEvent>>&& current_convo_events,
    uint32_t start_event_index,
    uint32_t end_event_index
) {
    RawConversation conversation = RawConversation::fromPrototype(prototype);

    conversation.events = move(current_convo_events);
    conversation.provenance = make_unique<EventRangeProvenance>(
        prototype.provenance->clone(),
        start_event_index,
        end_event_index
    );

    mut_conversations.push_back(move(conversation));
}

static CEDE(RawEvent) convert_event(
    IMM(YahooProtocolEvent) proto_event,
    uint32_t event_index,
    IMM(RawConversation) prototype
) {
    unique_ptr<RawEvent> event;

    ApparentTime timestamp = ApparentTime::fromUnixTimestamp(proto_event.timestamp);

    switch (proto_event.type) {
        case YahooProtocolEvent::Type::START_CONVERSATION:
            invariant(
                proto_event.text.isEmpty() && proto_event.extra.isEmpty(),
                "START_CONVERSATION events should have blank 'text' and 'extra' fields"
            );
            return make_unique<RawStartConversationEvent>(
                timestamp,
                event_index,
                implicit_subject(proto_event, prototype)
            );
        case YahooProtocolEvent::Type::CONFERENCE_JOIN:
            invariant(
                proto_event.direction == YahooProtocolEvent::Direction::INCOMING,
                "CONFERENCE_JOIN events can only be incoming"
            );
            event = make_unique<RawJoinConferenceEvent>(
                timestamp,
                event_index,
                parse_event_subject(proto_event, prototype)
            );
            if (!proto_event.text.isEmpty()) {
                ((RawJoinConferenceEvent*)event.get())->message = parse_message_content(proto_event.text);
            }
            return event;
        case YahooProtocolEvent::Type::CONFERENCE_DECLINE:
            invariant(
                proto_event.direction == YahooProtocolEvent::Direction::INCOMING,
                "CONFERENCE_DECLINE events can only be incoming"
            );
            event = make_unique<RawDeclineConferenceEvent>(
                timestamp,
                event_index,
                parse_event_subject(proto_event, prototype)
            );
            if (!proto_event.text.isEmpty()) {
                ((RawDeclineConferenceEvent*)event.get())->message = parse_message_content(proto_event.text);
            }
            return event;
        case YahooProtocolEvent::Type::CONFERENCE_LEAVE:
            invariant(
                proto_event.direction == YahooProtocolEvent::Direction::INCOMING,
                "CONFERENCE_LEAVE events can only be incoming"
            );
            event = make_unique<RawLeaveConferenceEvent>(
                timestamp,
                event_index,
                parse_event_subject(proto_event, prototype)
            );
            if (!proto_event.text.isEmpty()) {
                ((RawLeaveConferenceEvent*)event.get())->message = parse_message_content(proto_event.text);
            }
            return event;
        case YahooProtocolEvent::Type::CONVERSATION_MESSAGE:
            invariant(proto_event.extra.isEmpty(), "CONVERSATION_MESSAGE events should have no 'extra' field");
            // intentional fallthrough
        case YahooProtocolEvent::Type::CONFERENCE_MESSAGE:
            event = make_unique<RawMessageEvent>(
                timestamp,
                event_index,
                parse_event_subject(proto_event, prototype),
                parse_message_content(proto_event.text)
            );
            if ((proto_event.direction == YahooProtocolEvent::Direction::OUTGOING) && !proto_event.extra.isEmpty()) {
                ((RawMessageEvent*)event.get())->receiver = make_unique<AccountSubject>(
                    parse_yahoo_account(QString::fromUtf8(proto_event.extra))
                );
            }
            if (proto_event.direction == YahooProtocolEvent::Direction::OFFLINE) {
                ((RawMessageEvent*)event.get())->isOffline = true;
            }
            return event;
    }

    qDebug() << proto_event;
    invariant_violation("Event not converted");
}

static CEDE(ApparentSubject) implicit_subject(IMM(YahooProtocolEvent) proto_event, IMM(RawConversation) conversation) {
    return (proto_event.direction == YahooProtocolEvent::Direction::OUTGOING) ?
           conversation.identity->clone() : conversation.declaredPeers.front()->clone();
}

static CEDE(ApparentSubject) parse_event_subject(IMM(YahooProtocolEvent) proto_event, IMM(RawConversation) conversation) {
    if (proto_event.extra.isEmpty() || (proto_event.direction == YahooProtocolEvent::Direction::OUTGOING)) {
        return implicit_subject(proto_event, conversation);
    }

    return make_unique<AccountSubject>(parse_yahoo_account(QString::fromUtf8(proto_event.extra)));
}

static RawMessageContent parse_message_content(IMM(QByteArray) text_data) {
    RawMessageContent content;

    QString text = decode_utf8(text_data);

    QREGEX_CI(
        markup_pattern,
        "\\x1B\\[(?<pseudo_ansi_seq>[^m]+)m|"\
        "(?<html_tag></?(font)\\b[^>]*>)|"\
        "(?<yahoo_tag></?(fade|alt)\\b[^>]*>)"
    );
    auto iterator = markup_pattern.globalMatch(text_data);
    int last_text_pos = 0;

    while (iterator.hasNext()) {
        auto match = iterator.next();

        if (match.capturedStart(0) > last_text_pos) {
            content.addItem(make_text_section(text.mid(last_text_pos, match.capturedStart(0) - last_text_pos)));
        }

        content.addItem(parse_markup_tag(match));

        last_text_pos = match.capturedEnd(0);
    }

    if (text.length() > last_text_pos) {
        content.addItem(make_text_section(text.mid(last_text_pos, text.length() - last_text_pos)));
    }

    return content;
}

static CEDE(TextSection) make_text_section(IMM(QString) text) {
    invariant(!text.contains(0x1B), "Unprocessed ANSI-like sequence in text: \"%s\"", QP(text));

    return make_unique<TextSection>(text);
};

static CEDE(RawMessageContentItem) parse_markup_tag(IMM(QRegularExpressionMatch) match) {
    if (match.capturedLength("pseudo_ansi_seq")) {
        return parse_pseudo_ansi_seq(match.captured("pseudo_ansi_seq"));
    } else if (match.capturedLength("html_tag")) {
        return parse_html_tag(match.captured("html_tag"));
    } else if (match.capturedLength("yahoo_tag")) {
        return parse_yahoo_tag(match.captured("yahoo_tag"));
    }

    invariant_violation("Tag not recognized: \"%s\"", QP(match.captured(0)));
}

static CEDE(RawMessageContentItem) parse_pseudo_ansi_seq(IMM(QString) sgr_code) {
    QREGEX_MUST_MATCH_CI(
        match,
        "^(?<closed>x)?("\
        "(?<reset>0)|"\
        "(?<bold>1)|"\
        "(?<italic>2)|"\
        "(?<underline>4)|"\
        "(?<link>l)|"\
        "3(?<ansi_color>[0-8])|"\
        "(?<html_color>#[0-9a-f]{6})"\
        ")$",
        sgr_code,
        "SGR code not recognized: \"%s\""
    );

    bool closed = match.capturedLength("closed") > 0;

    if (match.capturedLength("reset")) {
        return make_unique<ANSIResetTag>();
    } else if (match.capturedLength("bold")) {
        return make_unique<BoldTag>(!closed);
    } else if (match.capturedLength("italic")) {
        return make_unique<ItalicTag>(!closed);
    } else if (match.capturedLength("underline")) {
        return make_unique<UnderlineTag>(!closed);
    } else if (match.capturedLength("link")) {
        return make_unique<LinkTag>(!closed);
    } else if (match.capturedLength("ansi_color")) {
        return make_unique<ANSIColorTag>(!closed, (ANSIColor)match.captured("ansi_color").toInt());
    } else if (match.capturedLength("html_color")) {
        return make_unique<RGBColorTag>(!closed, Color::fromHTMLFormat(match.captured("html_color")));
    }

    never_reached();
}

static CEDE(RawMessageContentItem) parse_html_tag(IMM(QString) tag_text) {
    ParsedHTMLTagInfo tag_info = parse_html_tag_lenient(tag_text);

    invariant(tag_info.valid, "Failed to parse tag: \"%s\"", QP(tag_text));
    invariant(
        !tag_info.open || !tag_info.closed,
        "Encountered unexpected self-closed tag: \"%s\"",
        QP(tag_text)
    );

    if (tag_info.tagName.toLower() == "font") {
        return parse_font_tag(tag_info);
    }

    invariant_violation("HTML tag not supported: \"%s\"", QP(tag_info.tagName));
}

static CEDE(FontTag) parse_font_tag(IMM(ParsedHTMLTagInfo) tag_info) {
    QREGEX(comma_separator, "\\s*,\\s*");

    auto tag = make_unique<FontTag>(tag_info.open);

    for (IMM(auto) key : tag_info.attributes.keys()) {
        QString norm_key = key.toLower();
        QString value = tag_info.attributes[key].trimmed();

        if (norm_key == "face") {
            tag->faces = vector<QString>();
            for (IMM(QString) item : value.split(comma_separator, QString::SkipEmptyParts)) {
                tag->faces->push_back(item);
            }
        } else if (norm_key == "size") {
            tag->size = value;
        } else {
            invariant_violation("Font attr not supported: \"%s\"", QP(key));
        }
    }

    return tag;
}

static CEDE(RawMessageContentItem) parse_yahoo_tag(IMM(QString) tag_text) {
#define PAT_COLOR "#[0-9a-f]{6}"
    QREGEX_MUST_MATCH_CI(
        match,
        "^<(?<closed>/)?(?<tag_name>[a-z._-][a-z0-9._-]*)\\b\\s*"\
        "(?<colors>" PAT_COLOR "(\\s*,\\s*" PAT_COLOR ")*)?\\s*>$",
        tag_text,
        "Yahoo tag not recognized: \"%s\""
    );

    QString tag_name = match.captured("tag_name").toLower();
    bool closed = match.capturedLength("closed") > 0;

    vector<Color> colors;
    if (match.capturedLength("colors")) {
        for (IMM(auto) color_str : match.captured("colors").split(',')) {
            colors.push_back(Color::fromHTMLFormat(color_str));
        }
    }

    if (tag_name == "fade") {
        invariant(closed || !colors.empty(), "<fade> tag must have at least one color specified");
        return closed ? make_unique<YahooFadeTag>(false) : make_unique<YahooFadeTag>(colors);
    } else if (tag_name == "alt") {
        invariant(closed || (colors.size() == 2), "<alt> tag must have exactly 2 colors specified");
        return closed ? make_unique<YahooAltTag>() : make_unique<YahooAltTag>(colors[0], colors[1]);
    }

    invariant_violation("Yahoo markup tag not supported: \"%s\"", QP(tag_name));
#undef PAT_COLOR
}

}}}
