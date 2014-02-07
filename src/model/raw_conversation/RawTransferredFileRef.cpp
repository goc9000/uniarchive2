#include "RawTransferredFileRef.h"


bool RawTransferredFileRef::isNull() const
{
    return filename.isNull();
}

QString RawTransferredFileRef::description() const
{
    if (isNull()) {
        return "<file unknown>";
    }

    return filename;
}

void RawTransferredFileRef::setNull()
{
    filename = QString::Null();
}
