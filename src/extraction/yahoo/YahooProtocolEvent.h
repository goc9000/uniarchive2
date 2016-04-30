/**
 * extraction/yahoo/YahooProtocolEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_YAHOO_YAHOOPROTOCOLEVENT_H
#define UNIARCHIVE2_EXTRACTION_YAHOO_YAHOOPROTOCOLEVENT_H

#include <QtGlobal>
#include <QByteArray>

#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace extraction { namespace yahoo {

struct YahooProtocolEvent {
    enum class Type {
        START_CONVERSATION = 0,
        CONVERSATION_MESSAGE = 6,
        CONFERENCE_JOIN = 25,
        CONFERENCE_DECLINE = 26,
        CONFERENCE_LEAVE = 27,
        CONFERENCE_MESSAGE = 29,
    };

    enum class Direction {
        OUTGOING = 0,
        INCOMING = 1,
        OFFLINE = 6,
    };

    quint32 timestamp;
    Type type;
    Direction direction;
    QByteArray text;
    QByteArray extra;

    YahooProtocolEvent(quint32 timestamp, Type type, Direction direction, IMM(QByteArray) text, IMM(QByteArray) extra)
        : timestamp(timestamp), type(type), direction(direction), text(text), extra(extra) {
    }
};

QDebug operator<< (QDebug stream, IMM(YahooProtocolEvent) event);

}}}

#endif //UNIARCHIVE2_EXTRACTION_YAHOO_YAHOOPROTOCOLEVENT_H
