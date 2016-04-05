/**
 * protocols/FullAccountName.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>
#include <QDebugStateSaver>

#include "protocols/FullAccountName.h"

namespace uniarchive2 { namespace protocols {

FullAccountName::FullAccountName()
    : protocol(IMProtocols::INVALID), accountName("") {
}

FullAccountName::FullAccountName(IMProtocols protocol, QString account_name)
    : protocol(protocol), accountName(account_name) {
}

QDebug operator<< (QDebug stream, const FullAccountName& account_name) {
    QDebugStateSaver settings(stream);

    stream.nospace() << qUtf8Printable(account_name.accountName) << "@" << account_name.protocol;

    return stream;
}

}}
