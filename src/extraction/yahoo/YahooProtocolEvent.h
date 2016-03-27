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

namespace uniarchive2 { namespace extraction { namespace yahoo {

struct YahooProtocolEvent {
    enum EventType {
        START_CONVERSATION = 0,
        CONVERSATION_MESSAGE = 6,
        CONFERENCE_JOIN = 25,
        CONFERENCE_DECLINE = 26,
        CONFERENCE_LEAVE = 27,
        CONFERENCE_MESSAGE = 29,
    };

    quint32 timestamp;
    quint32 type;
    quint32 direction;
    QByteArray text;
    QByteArray extra;

    YahooProtocolEvent(quint32 timestamp, quint32 type, quint32 direction, QByteArray text, QByteArray extra)
        : timestamp(timestamp), type(type), direction(direction), text(text), extra(extra) {
    }
};

QDebug operator<< (QDebug stream, const YahooProtocolEvent& event);

}}}

#endif //UNIARCHIVE2_EXTRACTION_YAHOO_YAHOOPROTOCOLEVENT_H
