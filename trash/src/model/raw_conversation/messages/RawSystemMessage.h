#ifndef RAWSYSTEMMESSAGE_H
#define RAWSYSTEMMESSAGE_H

#include "model/raw_conversation/messages/RawMessage.h"


class RawSystemMessage : public RawMessage
{
public:
    using RawMessage::RawMessage;
protected:
    QString _bodyDescription() const;
    virtual QString _sysMessageDescription() const = 0;
};

#endif // RAWSYSTEMMESSAGE_H
