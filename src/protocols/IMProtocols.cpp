/**
 * protocols/IMProtocols.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>

#include "protocols/IMProtocols.h"

namespace uniarchive2 { namespace protocols {

QString name_for_protocol(IMProtocols protocol) {
    switch (protocol) {
        case IMProtocols::INVALID:
            return "(invalid)";
        case IMProtocols::DIGSBY:
            return "Digsby";
        case IMProtocols::FACEBOOK:
            return "Facebook";
        case IMProtocols::JABBER:
            return "Jabber";
        case IMProtocols::MSN:
            return "MSN";
        case IMProtocols::WHATSAPP:
            return "WhatsApp";
        case IMProtocols::YAHOO:
            return "Yahoo";
    }
}

QDebug operator<< (QDebug stream, IMProtocols protocol) {
    stream << qPrintable(name_for_protocol(protocol));

    return stream;
}

}}
