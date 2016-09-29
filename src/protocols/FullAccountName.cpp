/**
 * protocols/FullAccountName.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "protocols/FullAccountName.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>
#include <QDebugStateSaver>

namespace uniarchive2 { namespace protocols {

FullAccountName::FullAccountName() : protocol(IMProtocol::INVALID), accountName("") {
}

FullAccountName::FullAccountName(IMProtocol protocol, IMM(QString) account_name)
    : protocol(protocol), accountName(account_name) {
}

void FullAccountName::serializeToStream(QDataStream& mut_stream) const {
    mut_stream << protocol << accountName;
}

bool FullAccountName::operator == (IMM(FullAccountName) other) const {
    return (protocol == other.protocol) && (accountName == other.accountName);
}

QDebug operator<< (QDebug stream, IMM(FullAccountName) account_name) {
    QDebugStateSaver settings(stream);

    stream.nospace() << QP(account_name.accountName) << "@" << account_name.protocol;

    return stream;
}

}}
