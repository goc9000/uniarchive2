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

#include "extraction/yahoo/extract_yahoo_messenger_conversations.h"
#include "extraction/yahoo/ExtractYahooProtocolEventsIterator.h"
#include "intermediate_format/ApparentTime.h"
#include "intermediate_format/events/IntermediateFormatEvent.h"
#include "intermediate_format/events/IFStartConversationEvent.h"
#include "intermediate_format/events/IFJoinConferenceEvent.h"
#include "intermediate_format/events/IFDeclineConferenceEvent.h"
#include "intermediate_format/events/IFLeaveConferenceEvent.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "protocols/ArchiveFormats.h"
#include "protocols/FullAccountName.h"
#include "protocols/yahoo/utils.h"
#include "utils/language/invariant.h"

using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::protocols::yahoo;

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
            break;
        default:
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
    if (proto_event.extra.isEmpty()) {
        return implicit_subject(proto_event, conversation);
    }

    return make_shared<SubjectGivenAsAccount>(parse_yahoo_account(QString::fromUtf8(proto_event.extra)));
}

}}}
