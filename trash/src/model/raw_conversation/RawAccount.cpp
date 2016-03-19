#include "RawAccount.h"


bool RawAccount::isNull() const
{
    return id.isNull();
}

QString RawAccount::description() const
{
    if (isNull()) {
        return "<account unknown>";
    }

    return id + ":" + IMProtocol_description(protocol);
}

void RawAccount::setNull()
{
    id = QString::Null();
    protocol = IMProtocol::INVALID;
}
