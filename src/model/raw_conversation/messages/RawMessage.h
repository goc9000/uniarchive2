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
    RawMessage(TimeStamp date, bool isOffline)
        : date(date), isOffline(isOffline) {}

    QString description() const;

    TimeStamp date;
    bool isOffline;
protected:
    virtual QString _bodyDescription() const = 0;
};

#endif // RAWMESSAGE_H
