/**
 * protocols/IMProtocol.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 533b6316669977ead986bb2bd5822bd0b4fd28852bbbbb6d2b09252df259b6b4
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_PROTOCOLS_IMPROTOCOL_H
#define UNIARCHIVE2_PROTOCOLS_IMPROTOCOL_H

#include <QDataStream>
#include <QDebug>
#include <QString>

namespace uniarchive2 { namespace protocols {

enum class IMProtocol : uint8_t {
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

QDataStream& operator>> (QDataStream& mut_stream, IMProtocol& mut_protocol);
QDataStream& operator<< (QDataStream& mut_stream, IMProtocol protocol);

}}

#endif //UNIARCHIVE2_PROTOCOLS_IMPROTOCOL_H
