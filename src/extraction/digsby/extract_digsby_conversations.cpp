/**
 * extraction/digsby/extract_digsby_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "extraction/digsby/extract_digsby_conversations.h"
#include "intermediate_format/content/formatting/BoldTag.h"
#include "intermediate_format/content/formatting/CSSStyleTag.h"
#include "intermediate_format/content/formatting/FontTag.h"
#include "intermediate_format/content/formatting/ItalicTag.h"
#include "intermediate_format/content/formatting/LinkTag.h"
#include "intermediate_format/content/formatting/UnderlineTag.h"
#include "intermediate_format/content/formatting/YahooFadeTag.h"
#include "intermediate_format/content/LineBreak.h"
#include "intermediate_format/content/TextSection.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/events/RawCorruptedMessageEvent.h"
#include "intermediate_format/events/RawMessageEvent.h"
#include "intermediate_format/provenance/ArchiveFileProvenance.h"
#include "intermediate_format/provenance/FileProvenance.h"
#include "intermediate_format/subjects/AccountSubject.h"
#include "protocols/digsby/digsby_account_name.h"
#include "protocols/FullAccountName.h"
#include "protocols/IMProtocol.h"
#include "protocols/parse_account_generic.h"
#include "utils/html/parse_html_lenient.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/text/decoding.h"

#include <QtDebug>
#include <QDir>
#include <QFileInfo>
#include <QIODevice>
#include <QLocale>
#include <QStringList>
#include <QTextCodec>
#include <QTextStream>
#include <QUrl>

namespace uniarchive2 { namespace extraction { namespace digsby {

using namespace std;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::provenance;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;
using namespace uniarchive2::protocols::digsby;
using namespace uniarchive2::utils::html;
using namespace uniarchive2::utils::text;

struct InfoFromFilename {
    FullAccountName identity;
    FullAccountName peer;
    bool isConference;
};

static RawConversation init_conversation(IMM(QString) filename);
static InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename);
static IMProtocol parse_protocol(IMM(QString) protocol_name);

static void verify_xml_header(QTextStream& mut_stream);
static void seek_start_of_events(QTextStream& mut_stream);
static QStringList partially_parse_events(QTextStream& mut_stream);

static CEDE(RawEvent) parse_event(IMM(QString) event_html, IMM(RawConversation) conversation);
static RawMessageContent parse_message_content(IMM(QString) content_html);
static CEDE(TextSection) parse_text_section(IMM(QString) text);
static CEDE(RawMessageContentItem) parse_markup_tag(IMM(ParsedHTMLTagInfo) tag_info);
static CEDE(FontTag) parse_font_tag(IMM(ParsedHTMLTagInfo) tag_info);


RawConversation extract_digsby_conversation(IMM(QString) filename) {
    RawConversation conversation = init_conversation(filename);

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qFatal("Can't open file: %s", QP(filename));
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

static RawConversation init_conversation(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", QP(filename));

    QString full_filename = file_info.absoluteFilePath();
    auto info = analyze_conversation_filename(full_filename);

    RawConversation conversation(info.identity.protocol);
    conversation.provenance =
        make_unique<ArchiveFileProvenance>(FileProvenance::fromQFileInfo(file_info), ArchiveFormat::DIGSBY);

    conversation.isConference = info.isConference;
    conversation.identity = make_unique<AccountSubject>(info.identity);
    conversation.declaredPeers.push_back(make_unique<AccountSubject>(info.peer));

    return conversation;
}

static InfoFromFilename analyze_conversation_filename(IMM(QString) full_filename) {
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
        QREGEX_MUST_MATCH_CI(
            group_chat_match, "^\\d{4}-\\d{2}-\\d{2}T\\d{2}[.]\\d{2}[.]\\d{2} - (.*)-\\d+[.]html$", base_name,
            "Digsby conference archive file must have a filename like \"YYYY-mm-ddThh.mm.ss - account_name-"\
            "<timestamp>.html\", it is actually \"%s\""
        );
        info.peer = parse_account_generic(protocol, group_chat_match.captured(1));
        info.isConference = true;
    } else {
        QREGEX_MUST_MATCH_CI(
            base_name_match, "^\\d{4}-\\d{2}-\\d{2}.html$", base_name,
            "Digsby archive file must have a filename like \"YYYY-mm-dd.html\", it is actually \"%s\""
        );
        QREGEX_MUST_MATCH_CI(
            remote_peer_match, "^(.*)_([a-z]+)$", peer_folder,
            "Digsby peer folder must look like <account>_<protocol>, it is actually \"%s\""
        );

        IMProtocol peer_protocol = parse_protocol(remote_peer_match.captured(2));
        info.peer = parse_account_generic(peer_protocol, remote_peer_match.captured(1));
        info.isConference = false;
    }

    return info;
}

static IMProtocol parse_protocol(IMM(QString) protocol_name) {
    const static QMap<QString, IMProtocol> PROTOCOL_MAP = {
        { "digsby", IMProtocol::DIGSBY },
        { "gtalk",  IMProtocol::JABBER },
        { "jabber", IMProtocol::JABBER },
        { "msn",    IMProtocol::MSN },
        { "yahoo",  IMProtocol::YAHOO },
    };

    if (PROTOCOL_MAP.contains(protocol_name)) {
        return PROTOCOL_MAP[protocol_name];
    }

    invariant_violation("Unrecognized protocol in Digsby: \"%s\"", QP(protocol_name));
}

static void verify_xml_header(QTextStream& mut_stream) {
    QString xml_header = mut_stream.readLine();
    invariant(
        xml_header.startsWith("<?xml ") && xml_header.contains("encoding=\"UTF-8\""),
        "Expected Digsby archive to be a UTF-8 encoded XML file, but header is \"%s\"",
        QP(xml_header)
    );

    mut_stream.setCodec(QTextCodec::codecForName("UTF-8"));
}

static void seek_start_of_events(QTextStream& mut_stream) {
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

static QStringList partially_parse_events(QTextStream& mut_stream) {
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

static CEDE(RawEvent) parse_event(IMM(QString) event_html, IMM(RawConversation) conversation) {
    QREGEX_MATCH(
        match,
        "^<div class=\"([^\"]*)\" auto=\"([^\"]*)\" timestamp=\"([^\"]*)\">\\s*"\
        "<span class=\"buddy\">([^<]*)</span>\\s*<span class=\"msgcontent\">\\s*(.*)</span></div>$",
        event_html
    );
    if (!match.hasMatch()) {
        return make_unique<RawCorruptedMessageEvent>(ApparentTime(), conversation.events.size(), event_html.toUtf8());
    }

    QStringList classes = match.captured(1).trimmed().split(QChar(' '), QString::SkipEmptyParts);
    invariant(classes.contains("message"), "Event of unsupported type (is not message): %s", QP(event_html));
    invariant(classes.contains("incoming") || classes.contains("outgoing"), "Missing class 'incoming' or 'outgoing'");

    QDateTime datetime = QLocale::c().toDateTime(match.captured(3), "yyyy-MM-dd HH:mm:ss");
    datetime.setTimeSpec(Qt::UTC);
    invariant(datetime.isValid(), "Invalid timestamp '%s' (must be yyyy-mm-dd hh:mm:ss)", QP(match.captured(3)));

    return make_unique<RawMessageEvent>(
        ApparentTime::fromQDateTime(datetime),
        conversation.events.size(),
        make_unique<AccountSubject>(parse_account_generic(conversation.protocol, match.captured(4))),
        parse_message_content(match.captured(5))
    );
}

static RawMessageContent parse_message_content(IMM(QString) content_html) {
    RawMessageContent content;

    auto lenient_parse_result = parse_html_lenient(content_html);

    for (size_t i = 0; i < lenient_parse_result.textSections.size(); i++) {
        if (i > 0) {
            content.addItem(parse_markup_tag(lenient_parse_result.tags[i-1]));
        }
        content.addItem(parse_text_section(lenient_parse_result.textSections[i]));
    }

    return content;
}

static CEDE(TextSection) parse_text_section(IMM(QString) text) {
    // Remove \n's because whenever these appear, <br>'s are also present
    QREGEX_WILL_MATCH(match, "^\n*(.*?)\n*$", text);

    QString trimmed = match.captured(1);
    if (trimmed.isEmpty()) {
        return unique_ptr<TextSection>();
    }

    return make_unique<TextSection>(trimmed);
}

static CEDE(RawMessageContentItem) parse_markup_tag(IMM(ParsedHTMLTagInfo) tag_info) {
    invariant(!tag_info.open || !tag_info.closed, "Did not expect self-closing tags in Digsby archives");

    if (tag_info.tagName == "br") {
        return make_unique<LineBreak>();
    } else if (tag_info.tagName == "b") {
        return make_unique<BoldTag>(tag_info.open);
    } else if (tag_info.tagName == "i") {
        return make_unique<ItalicTag>(tag_info.open);
    } else if (tag_info.tagName == "u") {
        return make_unique<UnderlineTag>(tag_info.open);
    } else if (tag_info.tagName == "span") {
        if (tag_info.closed) {
            return make_unique<CSSStyleTag>();
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
            return make_unique<LinkTag>(false);
        } else {
            return make_unique<LinkTag>(QUrl(tag_info.attributes["href"]));
        }
    } else if (tag_info.tagName == "fade") {
        return make_unique<YahooFadeTag>(tag_info.open);
    }

    if (tag_info.closed) {
        // Skip unrecognized closing tags (these are added by Digsby incorrectly)
        return unique_ptr<RawMessageContentItem>();
    }

    // If the tag is not recognized, return it as unparsed text
    return make_unique<TextSection>(tag_info.originalText);
}

static CEDE(FontTag) parse_font_tag(IMM(ParsedHTMLTagInfo) tag_info) {
    QREGEX(comma_separator, "\\s*,\\s*");

    invariant(tag_info.tagName == "font", "This function should be run on <font> tags only");

    auto font_tag = make_unique<FontTag>(tag_info.open);

    for (IMM(QString) key : tag_info.attributes.keys()) {
        QString value = tag_info.attributes[key].trimmed();
        if (key == "color") {
            font_tag->color = Color::fromHTMLFormat(value);
        } else if (key == "face") {
            font_tag->faces = vector<QString>();
            for (IMM(QString) item : value.split(comma_separator, QString::SkipEmptyParts)) {
                font_tag->faces->push_back(item);
            }
        } else if (key == "style") {
            font_tag->css = value;
        } else {
            invariant_violation("Unhandled <font> attribute \"%s\"", QP(key));
        }
    }

    return font_tag;
}

}}}
