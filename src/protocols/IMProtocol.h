/**
 * protocols/IMProtocol.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_PROTOCOLS_IMPROTOCOL_H
#define UNIARCHIVE2_PROTOCOLS_IMPROTOCOL_H

#include <QDebug>

namespace uniarchive2 { namespace protocols {

enum class IMProtocol {
    INVALID = 0,
    DIGSBY,
    FACEBOOK,
    JABBER,
    MSN,
    SKYPE,
    WHATSAPP,
    YAHOO,
};

QString name_for_im_protocol(IMProtocol protocol);

QDebug operator<< (QDebug stream, IMProtocol protocol);

}}

#endif //UNIARCHIVE2_PROTOCOLS_IMPROTOCOL_H
