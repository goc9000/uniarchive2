/**
 * extraction/pidgin/extract_pidgin_txt_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <vector>

#include <QtDebug>
#include <QStringList>

#include "extraction/pidgin/extract_pidgin_txt_conversations.h"
#include "extraction/pidgin/common_extract_pidgin_conversations_code.h"
#include "extraction/parse_libpurple_system_message.h"
#include "intermediate_format/events/RawMessageEvent.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "intermediate_format/ApparentTime.h"
#include "protocols/FullAccountName.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/text/load_text_file.h"
#include "utils/text/split_into_lines.h"
#include "utils/time/parse_date_parts.h"

using namespace std;
using namespace uniarchive2::extraction;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;
using namespace uniarchive2::utils::text;
using namespace uniarchive2::utils::time;

namespace uniarchive2 { namespace extraction { namespace pidgin {

struct PreParsedEvent {
    QString raw_timestamp;
    QString raw_screen_name;
    QString raw_content;
};

vector<PreParsedEvent> pre_parse_events(IMM(QString) text_data);
CEDE(RawEvent) parse_event(IMM(PreParsedEvent) raw_event, IMM(RawConversation) conversation);


RawConversation extract_pidgin_txt_conversation(IMM(QString) filename) {
    RawConversation conversation = init_conversation(filename, "txt", ArchiveFormat::PIDGIN_TXT);

    QString full_text = load_utf8_text_file(filename).trimmed();

    for (PreParsedEvent raw_event : pre_parse_events(full_text)) {
        conversation.events.push_back(parse_event(raw_event, conversation));
    }

    return conversation;
}

vector<PreParsedEvent> pre_parse_events(IMM(QString) text_data) {
    vector<PreParsedEvent> raw_events;

    if (text_data.isEmpty()) {
        return raw_events;
    }

    QStringList lines = split_into_lines(text_data);

    QREGEX_MUST_MATCH(
        header_match, "^Conversation with .* at .* on .* \\(.*\\)$", lines[0],
        "Unexpected header for Pidgin archive file: \"%s\""
    );

    PreParsedEvent raw_event;
    bool unsent_message_follows = false;

    for (int i = 1; i < lines.length(); i++) {
        QREGEX_MATCH(
            start_match,
            "^"\
            "\\((?<timestamp>[^)]*\\d+:\\d+:\\d+[^)]*)\\)"\
            "( (?!=Unable to send message)(?<screen_name>[^:]+): )?"\
            "\\s*(?<content>.*)"\
            "$",
            lines[i]
        );

        if (unsent_message_follows) {
            invariant(start_match.hasMatch(), "Expected unsent message to start with proper header");
            raw_event.raw_content.append("\n").append(start_match.captured("content"));
            unsent_message_follows = false;
        } else if (!start_match.hasMatch()) {
            invariant(i != 1, "First event in file does not match format!");
            raw_event.raw_content.append("\n").append(lines[i].trimmed());
        } else {
            if (i != 1) {
                raw_events.push_back(raw_event);
            }
            raw_event = PreParsedEvent();
            raw_event.raw_timestamp = start_match.captured("timestamp");
            raw_event.raw_screen_name = start_match.captured("screen_name");
            raw_event.raw_content = start_match.captured("content");

            if (raw_event.raw_screen_name.isEmpty() && raw_event.raw_content.startsWith("Message could not be sent")) {
                unsent_message_follows = true;
            }
        }

        if (i == lines.length() - 1) {
            raw_events.push_back(raw_event);
        }
    }

    return raw_events;
}

CEDE(RawEvent) parse_event(IMM(PreParsedEvent) raw_event, IMM(RawConversation) conversation) {
    ApparentTime timestamp = parse_timestamp(raw_event.raw_timestamp, conversation);

    if (raw_event.raw_screen_name.isEmpty()) {
        return parse_libpurple_system_message(
            (unsigned int)conversation.events.size(),
            timestamp,
            raw_event.raw_content,
            false,
            conversation.protocol
        );
    } else {
        return make_unique<RawMessageEvent>(
            timestamp,
            (unsigned int)conversation.events.size(),
            parse_libpurple_subject(raw_event.raw_screen_name, conversation.protocol, false),
            RawMessageContent::fromPlainText(raw_event.raw_content)
        );
    }

    return unique_ptr<RawEvent>();
}

}}}
