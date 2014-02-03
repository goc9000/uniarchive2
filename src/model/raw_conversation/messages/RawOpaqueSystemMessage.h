#ifndef RAWOPAQUESYSTEMMESSAGE_H
#define RAWOPAQUESYSTEMMESSAGE_H

#include "model/raw_conversation/messages/RawSystemMessage.h"


class RawOpaqueSystemMessage : public RawSystemMessage
{
public:
    RawOpaqueSystemMessage(TimeStamp date, bool isOffline, QString content)
        : RawSystemMessage(date, isOffline), content(content) {}

    QString content;
protected:
    QString _sysMessageDescription() const;
};

#endif // RAWOPAQUESYSTEMMESSAGE_H
