/**
 * extraction/digsby/extract_digsby_conversations.cpp
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
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>
#include <QUrl>

#include "extraction/digsby/extract_digsby_conversations.h"
#include "intermediate_format/content/TextSection.h"
#include "intermediate_format/content/BoldTag.h"
#include "intermediate_format/content/CSSStyleTag.h"
#include "intermediate_format/content/FontTag.h"
#include "intermediate_format/content/ItalicTag.h"
#include "intermediate_format/content/LineBreakTag.h"
#include "intermediate_format/content/LinkTag.h"
#include "intermediate_format/content/UnderlineTag.h"
#include "intermediate_format/content/YahooFadeTag.h"
#include "intermediate_format/events/IntermediateFormatEvent.h"
#include "intermediate_format/events/IFCorruptedMessageEvent.h"
#include "intermediate_format/events/IFMessageEvent.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "protocols/digsby/account_name.h"
#include "protocols/FullAccountName.h"
#include "protocols/IMProtocol.h"
#include "protocols/parse_account_generic.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/html/parse_html_lenient.h"
#include "utils/language/invariant.h"
#include "utils/text/decoding.h"

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;
using namespace uniarchive2::protocols::digsby;
using namespace uniarchive2::utils::html;
using namespace uniarchive2::utils::text;

namespace uniarchive2 { namespace extraction { namespace digsby {

struct InfoFromFilename {
    FullAccountName identity;
    FullAccountName peer;
    bool isConference;
};

IntermediateFormatConversation init_conversation(IMM(QString) filename);
InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename);
IMProtocol parse_protocol(IMM(QString) protocol_name);
FullAccountName parse_account(IMProtocol protocol, IMM(QString) account_name);

void verify_xml_header(QTextStream& mut_stream);
void seek_start_of_events(QTextStream& mut_stream);
QStringList partially_parse_events(QTextStream& mut_stream);

CEDE(IntermediateFormatEvent) parse_event(IMM(QString) event_html, IMM(IntermediateFormatConversation) conversation);
IntermediateFormatMessageContent parse_message_content(IMM(QString) content_html);
CEDE(TextSection) parse_text_section(IMM(QString) text);
CEDE(IntermediateFormatMessageContentItem) parse_markup_tag(IMM(ParsedHTMLTagInfo) tag_info);
CEDE(FontTag) parse_font_tag(IMM(ParsedHTMLTagInfo) tag_info);


IntermediateFormatConversation extract_digsby_conversation(IMM(QString) filename) {
    IntermediateFormatConversation conversation = init_conversation(filename);

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal("Can't open file: %s", qUtf8Printable(filename));
    }

    // Unfortunately we are reduced to manual text processing as the HTML produced by Digsby is occasionally damaged
    QTextStream stream(&file);

    verify_xml_header(stream);
    seek_start_of_events(stream);
    QStringList event_htmls = partially_parse_events(stream);

    for (IMM(QString) event_html : event_htmls) {
        conversation.events.push_back(parse_event(event_html, conversation));
    }

    return conversation;
}

IntermediateFormatConversation init_conversation(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", qUtf8Printable(filename));

    QString full_filename = file_info.absoluteFilePath();
    auto info = analyze_conversation_filename(full_filename);

    IntermediateFormatConversation conversation(ArchiveFormat::DIGSBY, info.identity.protocol);

    conversation.originalFilename = full_filename;
    conversation.fileLastModifiedTime = ApparentTime(
        file_info.lastModified().toTime_t(),
        ApparentTime::Reference::UNKNOWN
    );
    conversation.isConference = info.isConference;
    conversation.identity = make_unique<SubjectGivenAsAccount>(info.identity);
    conversation.declaredPeers.push_back(make_unique<SubjectGivenAsAccount>(info.peer));

    return conversation;
}

InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename) {
    InfoFromFilename info;

    QString logs_folder = full_filename.section(QDir::separator(), -6, -6);
    QString protocol_folder = full_filename.section(QDir::separator(), -4, -4);
    QString identity_folder = full_filename.section(QDir::separator(), -3, -3);
    QString peer_folder = full_filename.section(QDir::separator(), -2, -2);
    QString base_name = full_filename.section(QDir::separator(), -1, -1);

    invariant(
        logs_folder == "Digsby Logs",
        "Digsby archive file should be 5 levels deep under a \"Digsby Logs\" folder"
    );

    IMProtocol protocol = parse_protocol(protocol_folder);
    info.identity = parse_account_generic(protocol, identity_folder);

    if (peer_folder == "Group Chats") {
        info.isConference = true;
        static QRegularExpression pat_group_chat(
            "^\\d{4}-\\d{2}-\\d{2}T\\d{2}[.]\\d{2}[.]\\d{2} - (.*)-\\d+[.]html$",
            QRegularExpression::CaseInsensitiveOption
        );
        auto group_chat_match = pat_group_chat.match(base_name);
        invariant(
            group_chat_match.hasMatch(),
            "Digsby conference archive file must have a filename like "
                "YYYY-mm-ddThh.mm.ss - account_name-<timestamp>.html, it is actually \"%s\"",
            qUtf8Printable(base_name)
        );

        info.peer = parse_account_generic(protocol, group_chat_match.captured(1));
    } else {
        info.isConference = false;
        static QRegularExpression pat_base_name(
            "^\\d{4}-\\d{2}-\\d{2}.html$",
            QRegularExpression::CaseInsensitiveOption
        );
        invariant(
            pat_base_name.match(base_name).hasMatch(),
            "Digsby archive file must have a filename like YYYY-mm-dd.html, it is actually \"%s\"",
            qUtf8Printable(base_name)
        );

        static QRegularExpression pat_remote_peer("^(.*)_([a-z]+)$", QRegularExpression::CaseInsensitiveOption);
        auto remote_peer_match = pat_remote_peer.match(peer_folder);
        invariant(
            remote_peer_match.hasMatch(),
            "Digsby peer folder must look like <account>_<protocol>, it is actually \"%s\"",
            qUtf8Printable(peer_folder)
        );

        IMProtocol peer_protocol = parse_protocol(remote_peer_match.captured(2));
        info.peer = parse_account_generic(peer_protocol, remote_peer_match.captured(1));
    }

    return info;
}

IMProtocol parse_protocol(IMM(QString) protocol_name) {
    static QRegularExpression pattern("^((digsby)|(gtalk|jabber)|(msn)|(yahoo))$");

    switch (pattern.match(protocol_name).lastCapturedIndex() - 2) {
        case 0: return IMProtocol::DIGSBY;
        case 1: return IMProtocol::JABBER;
        case 2: return IMProtocol::MSN;
        case 3: return IMProtocol::YAHOO;
        default:
            invariant_violation("Unsupported protocol specified in Digsby: \"%s\"", qUtf8Printable(protocol_name));
    };
}

void verify_xml_header(QTextStream& mut_stream) {
    QString xml_header = mut_stream.readLine();
    invariant(
        xml_header.startsWith("<?xml ") && xml_header.contains("encoding=\"UTF-8\""),
        "Expected Digsby archive to be a UTF-8 encoded XML file, but header is \"%s\"",
        qUtf8Printable(xml_header)
    );

    mut_stream.setCodec(QTextCodec::codecForName("UTF-8"));
}

void seek_start_of_events(QTextStream& mut_stream) {
    while (!mut_stream.atEnd()) {
        QString line = mut_stream.readLine();

        if (line.trimmed() == "<BODY onload=\"utc_to_local()\">") {
            return;
        }
    }

    invariant_violation(
        "Could not find the <BODY onload=\"utc_to_local()\"> sequence that marks the start of messages"
    );
}

QStringList partially_parse_events(QTextStream& mut_stream) {
    QStringList event_htmls;
    QString current_event_html;
    bool event_open = false;

    while (!mut_stream.atEnd()) {
        QString line = mut_stream.readLine();

        if (line.startsWith("<div class=\"")) {
            if (event_open) {
                event_htmls << current_event_html;
            }
            current_event_html = line;
            event_open = true;
        } else if (event_open) {
            current_event_html.append("\n").append(line);
        } else {
            event_htmls << line;
            continue;
        }

        if (line.endsWith("</span></div>")) {
            event_htmls << current_event_html;
            event_open = false;
        }
    }

    if (event_open) {
        event_htmls << current_event_html;
    }

    return event_htmls;
}

CEDE(IntermediateFormatEvent) parse_event(IMM(QString) event_html, IMM(IntermediateFormatConversation) conversation) {
    static QRegularExpression pat_message_html(
        "^<div class=\"([^\"]*)\" auto=\"([^\"]*)\" timestamp=\"([^\"]*)\">\\s*"
            "<span class=\"buddy\">([^<]*)</span>\\s*<span class=\"msgcontent\">\\s*(.*)</span></div>$",
        QRegularExpression::DotMatchesEverythingOption
    );

    auto match = pat_message_html.match(event_html);
    if (!match.hasMatch()) {
        return make_unique<IFCorruptedMessageEvent>(ApparentTime(), conversation.events.size(), event_html.toUtf8());
    }

    QStringList classes = match.captured(1).trimmed().split(QChar(' '), QString::SkipEmptyParts);
    invariant(
        classes.contains("message"),
        "Event of unsupported type (is not message): %s", qUtf8Printable(event_html)
    );
    invariant(classes.contains("incoming") || classes.contains("outgoing"), "Missing class 'incoming' or 'outgoing'");

    QDateTime datetime = QDateTime::fromString(match.captured(3), "yyyy-MM-dd HH:mm:ss");
    datetime.setTimeSpec(Qt::UTC);
    invariant(
        datetime.isValid(),
        "Invalid timestamp '%s' (must be yyyy-mm-dd hh:mm:ss)",
        qUtf8Printable(match.captured(3))
    );

    return make_unique<IFMessageEvent>(
        ApparentTime(datetime),
        conversation.events.size(),
        make_unique<SubjectGivenAsAccount>(parse_account_generic(conversation.protocol, match.captured(4))),
        move(parse_message_content(match.captured(5)))
    );
}

IntermediateFormatMessageContent parse_message_content(IMM(QString) content_html) {
    IntermediateFormatMessageContent content;

    auto lenient_parse_result = parse_html_lenient(content_html);

    for (int i = 0; i < lenient_parse_result.textSections.size(); i++) {
        if (i > 0) {
            content.addItem(parse_markup_tag(lenient_parse_result.tags[i-1]));
        }
        content.addItem(parse_text_section(lenient_parse_result.textSections[i]));
    }

    return content;
}

CEDE(TextSection) parse_text_section(IMM(QString) text) {
    // Remove \n's because whenever these appear, <br>'s are also present
    static QRegularExpression trim_pattern("^\n*(.*?)\n*$");
    auto match = trim_pattern.match(text);
    invariant(match.hasMatch(), "Expected to always get match here");

    QString trimmed = match.captured(1);
    if (trimmed.isEmpty()) {
        return unique_ptr<TextSection>();
    }

    return make_unique<TextSection>(trimmed);
}

CEDE(IntermediateFormatMessageContentItem) parse_markup_tag(IMM(ParsedHTMLTagInfo) tag_info) {
    invariant(!tag_info.open || !tag_info.closed, "Did not expect self-closing tags in Digsby archives");

    if (tag_info.tagName == "br") {
        return make_unique<LineBreakTag>();
    } else if (tag_info.tagName == "b") {
        return make_unique<BoldTag>(tag_info.closed);
    } else if (tag_info.tagName == "i") {
        return make_unique<ItalicTag>(tag_info.closed);
    } else if (tag_info.tagName == "u") {
        return make_unique<UnderlineTag>(tag_info.closed);
    } else if (tag_info.tagName == "span") {
        if (tag_info.closed) {
            return make_unique<CSSStyleTag>(true);
        } else {
            invariant(
                tag_info.attributes.empty() ||
                ((tag_info.attributes.size() == 1) && (tag_info.attributes.keys().first() == "style")),
                "Expected <span style=\"(css)\"> to be the only use of the SPAN tag"
            );
            return make_unique<CSSStyleTag>(tag_info.attributes["style"]);
        }
    } else if (tag_info.tagName == "font") {
        return parse_font_tag(tag_info);
    } else if (tag_info.tagName == "a") {
        if (tag_info.closed) {
            return make_unique<LinkTag>(true);
        } else {
            return make_unique<LinkTag>(QUrl(tag_info.attributes["href"]));
        }
    } else if (tag_info.tagName == "fade") {
        return make_unique<YahooFadeTag>(tag_info.closed);
    }

    if (tag_info.closed) {
        // Skip unrecognized closing tags (these are added by Digsby incorrectly)
        return unique_ptr<IntermediateFormatMessageContentItem>();
    }

    // If the tag is not recognized, return it as unparsed text
    return make_unique<TextSection>(tag_info.originalText);
}

CEDE(FontTag) parse_font_tag(IMM(ParsedHTMLTagInfo) tag_info) {
    invariant(tag_info.tagName == "font", "This function should be run on <font> tags only");

    auto font_tag = make_unique<FontTag>(tag_info.closed);

    for (IMM(QString) key : tag_info.attributes.keys()) {
        QString value = tag_info.attributes[key].trimmed();
        if (key == "color") {
            font_tag->color = Color::fromHTMLFormat(value);
        } else if (key == "face") {
            font_tag->faces = value.split(QRegularExpression("\\s*,\\s*"), QString::SkipEmptyParts);
        } else if (key == "style") {
            font_tag->css = value;
        } else {
            invariant_violation("Unhandled <font> attribute \"%s\"", qUtf8Printable(key));
        }
    }

    return font_tag;
}

}}}
