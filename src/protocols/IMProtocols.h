/**
 * protocols/IMProtocols.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_PROTOCOLS_IMPROTOCOLS_H
#define UNIARCHIVE2_PROTOCOLS_IMPROTOCOLS_H

#include <QDebug>

namespace uniarchive2 { namespace protocols {

enum class IMProtocols {
    INVALID = 0,
    DIGSBY,
    FACEBOOK,
    JABBER,
    MSN,
    WHATSAPP,
    YAHOO,
};

QDebug operator<< (QDebug stream, IMProtocols protocol);

}}

#endif //UNIARCHIVE2_PROTOCOLS_IMPROTOCOLS_H
