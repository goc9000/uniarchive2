/**
 * protocols/FullAccountName.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_PROTOCOLS_FULLACCOUNTNAME_H
#define UNIARCHIVE2_PROTOCOLS_FULLACCOUNTNAME_H

#include <QDebug>
#include <QString>

#include "protocols/IMProtocols.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace protocols {

struct FullAccountName {
public:
    IMProtocols protocol;
    QString accountName;

    FullAccountName();
    FullAccountName(IMProtocols protocol, IMM(QString) account_name);
};

QDebug operator<< (QDebug stream, IMM(FullAccountName) account_name);

}}

#endif //UNIARCHIVE2_PROTOCOLS_FULLACCOUNTNAME_H
