/**
 * protocols/IMProtocol.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "protocols/IMProtocol.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace protocols {

QString name_for_im_protocol(IMProtocol protocol) {
    switch (protocol) {
        case IMProtocol::INVALID:
            return "(invalid)";
        case IMProtocol::DIGSBY:
            return "Digsby";
        case IMProtocol::FACEBOOK:
            return "Facebook";
        case IMProtocol::JABBER:
            return "Jabber";
        case IMProtocol::MSN:
            return "MSN";
        case IMProtocol::SKYPE:
            return "Skype";
        case IMProtocol::WHATSAPP:
            return "WhatsApp";
        case IMProtocol::YAHOO:
            return "Yahoo";
    }
}

QDebug operator<< (QDebug stream, IMProtocol protocol) {
    stream << QP(name_for_im_protocol(protocol));

    return stream;
}

}}
