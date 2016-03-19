#ifndef RAWACCOUNT_H
#define RAWACCOUNT_H

#include <QString>

#include <memory>

#include "model/constants.h"


class RawAccount;
typedef std::unique_ptr<RawAccount> RawAccountUqPtr;

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
