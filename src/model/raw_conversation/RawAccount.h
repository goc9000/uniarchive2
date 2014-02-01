#ifndef RAWACCOUNT_H
#define RAWACCOUNT_H

#include <QString>

#include "model/constants.h"


class RawAccount
{
public:
    RawAccount() : id(QString::Null()), protocol(IMProtocol::INVALID) {}
    RawAccount(QString id, IMProtocol protocol) : id(id), protocol(protocol) {}

    QString id;
    IMProtocol protocol;

    bool isNull() const;

    QString description() const;

    void setNull();
};

#endif // RAWACCOUNT_H
