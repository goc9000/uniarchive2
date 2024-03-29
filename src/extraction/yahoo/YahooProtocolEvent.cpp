/**
 * extraction/yahoo/YahooProtocolEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "extraction/yahoo/YahooProtocolEvent.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>
#include <QDebugStateSaver>

namespace uniarchive2 { namespace extraction { namespace yahoo {

QString name_for_event_type(YahooProtocolEvent::Type type) {
    switch (type) {
        case YahooProtocolEvent::Type::START_CONVERSATION:
            return "StartConversation";
        case YahooProtocolEvent::Type::CONVERSATION_MESSAGE:
            return "ConversationMessage";
        case YahooProtocolEvent::Type::CONFERENCE_JOIN:
            return "JoinConference";
        case YahooProtocolEvent::Type::CONFERENCE_DECLINE:
            return "DeclineConference";
        case YahooProtocolEvent::Type::CONFERENCE_LEAVE:
            return "LeaveConference";
        case YahooProtocolEvent::Type::CONFERENCE_MESSAGE:
            return "ConferenceMessage";
    }
}

QString name_for_event_direction(YahooProtocolEvent::Direction direction) {
    switch (direction) {
        case YahooProtocolEvent::Direction::OUTGOING:
            return "outgoing";
        case YahooProtocolEvent::Direction::INCOMING:
            return "incoming";
        case YahooProtocolEvent::Direction::OFFLINE:
            return "offline";
    }
}

QDebug operator<< (QDebug stream, IMM(YahooProtocolEvent) event) {
    QDebugStateSaver settings(stream);
    stream.nospace() << "YahooProtocolEvent(";
    stream << "timestamp=" << event.timestamp;
    stream << ", type=" << QP(name_for_event_type(event.type));
    stream << ", dir=" << QP(name_for_event_direction(event.direction));
    stream << ", text=" << event.text;
    stream << ", extra=" << event.extra;
    stream << ")";
    return stream;
}

}}}
