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

#include "protocols/IMProtocol.h"
#include "utils/language/shortcuts.h"
#include "utils/serialization/IDeserializableStatic.h"
#include "utils/serialization/ISerializable.h"

#include <QDebug>
#include <QString>

namespace uniarchive2 { namespace protocols {

using namespace uniarchive2::utils::serialization;

struct FullAccountName final : ISerializable, IDeserializableStatic {
public:
    IMProtocol protocol;
    QString accountName;

    FullAccountName();
    FullAccountName(IMProtocol protocol, IMM(QString) account_name);

    static FullAccountName deserializeFromStream(QDataStream& mut_stream);
    void serializeToStream(QDataStream& mut_stream) const;

    bool operator == (IMM(FullAccountName) other) const;
};

QDebug operator<< (QDebug stream, IMM(FullAccountName) account_name);

}}

#endif //UNIARCHIVE2_PROTOCOLS_FULLACCOUNTNAME_H
