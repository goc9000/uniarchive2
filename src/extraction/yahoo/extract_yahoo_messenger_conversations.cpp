/**
 * extraction/yahoo/extract_yahoo_messenger_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <memory>

#include <QtDebug>
#include <QDir>
#include <QFile>
#include <QRegularExpression>

#include "graphics/Color.h"
#include "graphics/ANSIColors.h"
#include "extraction/yahoo/extract_yahoo_messenger_conversations.h"
#include "extraction/yahoo/ExtractYahooProtocolEventsIterator.h"
#include "intermediate_format/ApparentTime.h"
#include "intermediate_format/content/IntermediateFormatMessageContent.h"
#include "intermediate_format/content/TextSection.h"
#include "intermediate_format/content/BoldTag.h"
#include "intermediate_format/content/ItalicTag.h"
#include "intermediate_format/content/UnderlineTag.h"
#include "intermediate_format/content/LinkTag.h"
#include "intermediate_format/content/ANSIColorTag.h"
#include "intermediate_format/content/RGBColorTag.h"
#include "intermediate_format/content/ANSIResetTag.h"
#include "intermediate_format/content/FontTag.h"
#include "intermediate_format/content/YahooAltTag.h"
#include "intermediate_format/content/YahooFadeTag.h"
#include "intermediate_format/events/IntermediateFormatEvent.h"
#include "intermediate_format/events/IFStartConversationEvent.h"
#include "intermediate_format/events/IFJoinConferenceEvent.h"
#include "intermediate_format/events/IFDeclineConferenceEvent.h"
#include "intermediate_format/events/IFLeaveConferenceEvent.h"
#include "intermediate_format/events/IFMessageEvent.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "protocols/ArchiveFormats.h"
#include "protocols/FullAccountName.h"
#include "protocols/yahoo/account_name.h"
#include "utils/language/invariant.h"
#include "utils/html/parse_html_tag_lenient.h"
#include "utils/text/decoding.h"

using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::protocols::yahoo;
using namespace uniarchive2::utils::html;
using namespace uniarchive2::utils::text;

using namespace std;

namespace uniarchive2 { namespace extraction { namespace yahoo {

IntermediateFormatConversation build_conversation_prototype(const QString& filename);
QVector<shared_ptr<IntermediateFormatEvent>> convert_event(
    const YahooProtocolEvent& proto_event,
    const IntermediateFormatConversation& conversation
);
shared_ptr<SubjectGivenAsAccount> implicit_subject(
    const YahooProtocolEvent& proto_event,
    const IntermediateFormatConversation& conversation
);
shared_ptr<SubjectGivenAsAccount> parse_event_subject(
    const YahooProtocolEvent& proto_event,
    const IntermediateFormatConversation& conversation
);
IntermediateFormatMessageContent parse_message_content(const QByteArray& text_data);
shared_ptr<IntermediateFormatMessageContentItem> parse_markup_tag(const QRegularExpressionMatch& match);
shared_ptr<IntermediateFormatMessageContentItem> parse_pseudo_ansi_seq(const QString& sgr_code);
shared_ptr<IntermediateFormatMessageContentItem> parse_html_tag(const QString& tag_text);
shared_ptr<FontTag> parse_font_tag(bool closed, const QMap<QString, QString>& attributes);
shared_ptr<IntermediateFormatMessageContentItem> parse_yahoo_tag(const QString& tag_text);


QVector<IntermediateFormatConversation> extract_yahoo_messenger_conversations(const QString& filename) {
    QVector<IntermediateFormatConversation> conversations;
    IntermediateFormatConversation prototype = build_conversation_prototype(filename);

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal("Can't open file: %s", qUtf8Printable(filename));
    }
    QByteArray data = file.readAll();

    ExtractYahooProtocolEventsIterator proto_events(data, prototype.localAccount.accountName);

    while (proto_events.hasNext()) {
        prototype.events += convert_event(proto_events.next(), prototype);
    }

    conversations.append(prototype);

    return conversations;
}

IntermediateFormatConversation build_conversation_prototype(const QString& filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", qUtf8Printable(filename));

    QString full_filename = file_info.absoluteFilePath();

    static QRegularExpression pattern("\\d{8}-(.+)[.]dat", QRegularExpression::CaseInsensitiveOption);
    auto match = pattern.match(filename.section(QDir::separator(), -1, -1));
    invariant(match.hasMatch(), "Yahoo archive filename does not have the form YYYYMMDD-account_name.dat");
    auto local_account = parse_yahoo_account(match.captured(1));

    IntermediateFormatConversation prototype(ArchiveFormats::YAHOO_MESSENGER, local_account);

    prototype.originalFilename = full_filename;
    prototype.fileLastModifiedTime =
        ApparentTime(file_info.lastModified().toTime_t(), ApparentTime::Reference::UNKNOWN);

    auto remote_account = parse_yahoo_account(full_filename.section(QDir::separator(), -2, -2));
    prototype.declaredRemoteAccounts.push_back(remote_account);

    QString top_folder = full_filename.section(QDir::separator(), -3, -3);
    if (top_folder == "Messages") {
        prototype.isConference = false;
    } else if (top_folder == "Conferences") {
        prototype.isConference = true;
    } else {
        invariant_violation("Yahoo Messenger archive file must be in a 'Messages' or 'Conferences' folder");
    }

    return prototype;
}

QVector<shared_ptr<IntermediateFormatEvent>> convert_event(
    const YahooProtocolEvent& proto_event,
    const IntermediateFormatConversation& conversation
) {
    QVector<shared_ptr<IntermediateFormatEvent>> events;
    shared_ptr<IntermediateFormatEvent> event;
    shared_ptr<IntermediateFormatEvent> extra_event;
    bool extra_event_after = true;

    ApparentTime timestamp(proto_event.timestamp, ApparentTime::Reference::UTC);
    unsigned int next_index = (unsigned int)conversation.events.length();

    switch (proto_event.type) {
        case YahooProtocolEvent::Type::START_CONVERSATION:
            invariant(
                proto_event.text.isEmpty() && proto_event.extra.isEmpty(),
                "START_CONVERSATION events should have blank 'text' and 'extra' fields"
            );
            event = make_shared<IFStartConversationEvent>(
                timestamp,
                next_index,
                implicit_subject(proto_event, conversation)
            );
            break;
        case YahooProtocolEvent::Type::CONFERENCE_JOIN:
            invariant(
                proto_event.direction == YahooProtocolEvent::Direction::INCOMING,
                "CONFERENCE_JOIN events can only be incoming"
            );
            event = make_shared<IFJoinConferenceEvent>(
                timestamp,
                next_index,
                parse_event_subject(proto_event, conversation)
            );
            if (!proto_event.text.isEmpty()) {
                ((IFJoinConferenceEvent*)event.get())->message = parse_message_content(proto_event.text);
            }
            break;
        case YahooProtocolEvent::Type::CONFERENCE_DECLINE:
            invariant(
                proto_event.direction == YahooProtocolEvent::Direction::INCOMING,
                "CONFERENCE_DECLINE events can only be incoming"
            );
            event = make_shared<IFDeclineConferenceEvent>(
                timestamp,
                next_index,
                parse_event_subject(proto_event, conversation)
            );
            if (!proto_event.text.isEmpty()) {
                ((IFDeclineConferenceEvent*)event.get())->message = parse_message_content(proto_event.text);
            }
            break;
        case YahooProtocolEvent::Type::CONFERENCE_LEAVE:
            invariant(
                proto_event.direction == YahooProtocolEvent::Direction::INCOMING,
                "CONFERENCE_LEAVE events can only be incoming"
            );
            event = make_shared<IFLeaveConferenceEvent>(
                timestamp,
                next_index,
                parse_event_subject(proto_event, conversation)
            );
            if (!proto_event.text.isEmpty()) {
                ((IFLeaveConferenceEvent*)event.get())->message = parse_message_content(proto_event.text);
            }
            break;
        case YahooProtocolEvent::Type::CONVERSATION_MESSAGE:
            invariant(proto_event.extra.isEmpty(), "CONVERSATION_MESSAGE events should have no 'extra' field");
            // intentional fallthrough
        case YahooProtocolEvent::Type::CONFERENCE_MESSAGE:
            event = make_shared<IFMessageEvent>(
                timestamp,
                next_index,
                parse_event_subject(proto_event, conversation),
                parse_message_content(proto_event.text)
            );

            if ((proto_event.direction == YahooProtocolEvent::Direction::OUTGOING) && !proto_event.extra.isEmpty()) {
                ((IFMessageEvent*)event.get())->receiver = make_shared<SubjectGivenAsAccount>(
                    parse_yahoo_account(QString::fromUtf8(proto_event.extra))
                );
            }
            // TODO: offline messages
            break;
    }

    if (!event) {
//        qDebug() << proto_event;
//        invariant_violation("Event not converted");

        return events;
    } else {
        events.append(event);
    }

    return events;
}

shared_ptr<SubjectGivenAsAccount> implicit_subject(
    const YahooProtocolEvent& proto_event,
    const IntermediateFormatConversation& conversation
) {
    return make_shared<SubjectGivenAsAccount>(
        (proto_event.direction == YahooProtocolEvent::Direction::OUTGOING) ?
        conversation.localAccount : conversation.declaredRemoteAccounts.first()
    );
}

shared_ptr<SubjectGivenAsAccount> parse_event_subject(
    const YahooProtocolEvent& proto_event,
    const IntermediateFormatConversation& conversation
) {
    if (proto_event.extra.isEmpty() || (proto_event.direction == YahooProtocolEvent::Direction::OUTGOING)) {
        return implicit_subject(proto_event, conversation);
    }

    return make_shared<SubjectGivenAsAccount>(parse_yahoo_account(QString::fromUtf8(proto_event.extra)));
}

IntermediateFormatMessageContent parse_message_content(const QByteArray& text_data) {
    static QRegularExpression markup_pattern(
        "\\x1B\\[(?<pseudo_ansi_seq>[^m]+)m|"\
        "(?<html_tag></?(font)\\b[^>]*>)|"\
        "(?<yahoo_tag></?(fade|alt)\\b[^>]*>)",
        QRegularExpression::CaseInsensitiveOption
    );

    IntermediateFormatMessageContent content;

    QString text = decode_utf8(text_data);

    auto iterator = markup_pattern.globalMatch(text_data);
    int last_text_pos = 0;

    while (iterator.hasNext()) {
        auto match = iterator.next();

        if (match.capturedStart(0) > last_text_pos) {
            content.items.append(
                make_shared<TextSection>(text.mid(last_text_pos, match.capturedStart(0) - last_text_pos))
            );
        }

        auto markup_tag = parse_markup_tag(match);
        if (markup_tag) {
            content.items.append(markup_tag);
        }

        last_text_pos = match.capturedEnd(0);
    }

    if (text.length() > last_text_pos) {
        content.items.append(make_shared<TextSection>(text.mid(last_text_pos, text.length() - last_text_pos)));
    }

    return content;
}

shared_ptr<IntermediateFormatMessageContentItem> parse_markup_tag(const QRegularExpressionMatch& match) {
    if (match.capturedLength("pseudo_ansi_seq")) {
        return parse_pseudo_ansi_seq(match.captured("pseudo_ansi_seq"));
    } else if (match.capturedLength("html_tag")) {
        return parse_html_tag(match.captured("html_tag"));
    } else if (match.capturedLength("yahoo_tag")) {
        return parse_yahoo_tag(match.captured("yahoo_tag"));
    }

    invariant_violation("Tag not recognized: \"%s\"", qUtf8Printable(match.captured(0)));
}

shared_ptr<IntermediateFormatMessageContentItem> parse_pseudo_ansi_seq(const QString& sgr_code) {
    static QRegularExpression pattern(
        "^(?<closed>x)?("\
        "(?<reset>0)|"\
        "(?<bold>1)|"\
        "(?<italic>2)|"\
        "(?<underline>4)|"\
        "(?<link>l)|"\
        "3(?<ansi_color>[0-8])|"\
        "(?<html_color>#[0-9a-f]{6})"\
        ")$",
        QRegularExpression::CaseInsensitiveOption
    );

    auto match = pattern.match(sgr_code);
    invariant(match.hasMatch(), "SGR code not recognized: \"%s\"", qUtf8Printable(sgr_code));

    bool closed = match.capturedLength("closed") > 0;

    if (match.capturedLength("reset")) {
        return make_shared<ANSIResetTag>();
    } else if (match.capturedLength("bold")) {
        return make_shared<BoldTag>(closed);
    } else if (match.capturedLength("italic")) {
        return make_shared<ItalicTag>(closed);
    } else if (match.capturedLength("underline")) {
        return make_shared<UnderlineTag>(closed);
    } else if (match.capturedLength("link")) {
        return make_shared<LinkTag>(closed);
    } else if (match.capturedLength("ansi_color")) {
        return make_shared<ANSIColorTag>((ANSIColors)match.captured("ansi_color").toInt(), closed);
    } else if (match.capturedLength("html_color")) {
        return make_shared<RGBColorTag>(Color::fromHTMLFormat(match.captured("html_color")), closed);
    }

    never_reached();
}

shared_ptr<IntermediateFormatMessageContentItem> parse_html_tag(const QString& tag_text) {
    QString tag_name;
    bool open;
    bool closed;
    QMap<QString, QString> attributes;

    invariant(
        parse_html_tag_lenient(tag_text, tag_name, open, closed, attributes),
        "Failed to parse tag: \"%s\"",
        qUtf8Printable(tag_text)
    );
    invariant(!open || !closed, "Encountered unexpected self-closed tag: \"%s\"", qUtf8Printable(tag_text));

    tag_name = tag_name.toLower();

    if (tag_name == "font") {
        return parse_font_tag(closed, attributes);
    }

    invariant_violation("HTML tag not supported: \"%s\"", qUtf8Printable(tag_name));
}

shared_ptr<FontTag> parse_font_tag(bool closed, const QMap<QString, QString>& attributes) {
    auto tag = make_shared<FontTag>(closed);
    bool ok = false;

    for (const auto& key : attributes.keys()) {
        QString norm_key = key.toLower();
        QString value = attributes[key].trimmed();

        if (norm_key == "face") {
            tag->faces = value.split(QRegularExpression("\\s*,\\s*"), QString::SkipEmptyParts);
        } else if (norm_key == "size") {
            tag->size = value;
        } else {
            invariant_violation("Font attr not supported: \"%s\"", qUtf8Printable(key));
        }
    }

    return tag;
}

shared_ptr<IntermediateFormatMessageContentItem> parse_yahoo_tag(const QString& tag_text) {
#define PAT_COLOR "#[0-9a-f]{6}"
    static QRegularExpression pattern(
        "^<(?<closed>/)?(?<tag_name>[a-z._-][a-z0-9._-]*)\\b\\s*(?<colors>" PAT_COLOR "(\\s*,\\s*" PAT_COLOR ")*)?\\s*>$",
        QRegularExpression::CaseInsensitiveOption
    );

    auto match = pattern.match(tag_text);
    invariant(match.hasMatch(), "Yahoo tag not recognized: \"%s\"", qUtf8Printable(tag_text));

    QString tag_name = match.captured("tag_name").toLower();
    bool closed = match.capturedLength("closed") > 0;

    QList<Color> colors;
    if (match.capturedLength("colors")) {
        for (const auto& color_str : match.captured("colors").split(',')) {
            colors.append(Color::fromHTMLFormat(color_str));
        }
    }

    if (tag_name == "fade") {
        invariant(closed || (colors.length() > 0), "<fade> tag must have at least one color specified");
        return make_shared<YahooFadeTag>(closed, colors);
    } else if (tag_name == "alt") {
        invariant(closed || (colors.length() == 2), "<alt> tag must have exactly 2 colors specified");
        return closed ?
               make_shared<YahooAltTag>(true) :
               make_shared<YahooAltTag>(false, colors[0], colors[1]);
    }

    invariant_violation("Yahoo markup tag not supported: \"%s\"", qUtf8Printable(tag_name));
#undef PAT_COLOR
}

}}}
