#ifndef RAWTRANSFERREDFILEREF_H
#define RAWTRANSFERREDFILEREF_H

#include <QString>

#include <memory>

#include "model/raw_conversation/RawAccount.h"


class RawTransferredFileRef;
typedef std::unique_ptr<RawTransferredFileRef> RawTransferredFileRefUqPtr;

class RawTransferredFileRef
{
public:
    RawTransferredFileRef()
        : filename(QString::Null()) {}
    RawTransferredFileRef(QString filename)
        : filename(filename) {}

    QString filename;

    bool isNull() const;

    QString description() const;

    void setNull();
};

#endif // RAWTRANSFERREDFILEREF_H
