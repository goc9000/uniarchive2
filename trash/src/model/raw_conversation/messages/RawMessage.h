#ifndef RAWMESSAGE_H
#define RAWMESSAGE_H

#include <QString>

#include <memory>

#include "model/time/TimeStamp.h"


class RawMessage;
typedef std::unique_ptr<RawMessage> RawMessageUqPtr;

class RawMessage
{
public:
    enum class Flags
    {
        OFFLINE    = (1 << 0)
    };

    RawMessage(TimeStamp date, Flags flags)
        : date(date), flags(flags) {}

    bool isOffline() const;

    QString description() const;

    TimeStamp date;
    Flags flags;
protected:
    virtual QString _bodyDescription() const = 0;
};

#endif // RAWMESSAGE_H
