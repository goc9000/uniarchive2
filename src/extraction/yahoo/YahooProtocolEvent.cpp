/**
 * extraction/yahoo/YahooProtocolEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>
#include <QDebugStateSaver>

#include "extraction/yahoo/YahooProtocolEvent.h"

namespace uniarchive2 { namespace extraction { namespace yahoo {

QDebug operator<< (QDebug stream, const YahooProtocolEvent& event) {
    QDebugStateSaver settings(stream);
    stream.nospace() << "YahooProtocolEvent(";
    stream << "timestamp=" << event.timestamp;
    stream << ", type=" << event.type;
    stream << ", dir=" << event.direction;
    stream << ", text=" << event.text;
    stream << ", extra=" << event.extra;
    stream << ")";
    return stream;
}

}}}
