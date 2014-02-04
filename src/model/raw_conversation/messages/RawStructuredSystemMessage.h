#ifndef RAWSTRUCTUREDSYSTEMMESSAGE_H
#define RAWSTRUCTUREDSYSTEMMESSAGE_H

#include "model/raw_conversation/messages/RawSystemMessage.h"
#include "model/raw_conversation/RawSpeaker.h"
#include "model/constants.h"


class RawStructuredSystemMessage : public RawSystemMessage
{
public:
    class Param
    {
    public:
        enum Type { EMPTY, SPEAKER };

        Param() : tag(EMPTY) {}
        Param(RawSpeaker* speaker)
            : tag(SPEAKER), speaker(speaker) {}

        bool isNull() const;
        Type type() const;

        QString description() const;
    protected:
        Type tag;
        union {
            RawSpeaker* speaker;
        };
    };

    RawStructuredSystemMessage(TimeStamp date, bool isOffline,
                               Param subject, SystemMessagePredicate predicate,
                               Param object=Param(), Param aux=Param())
        : RawSystemMessage(date, isOffline), subject(subject),
          predicate(predicate), object(object), aux(aux) {}

    Param subject;
    SystemMessagePredicate predicate;
    Param object;
    Param aux;
protected:
    QString _sysMessageDescription() const;
};

#endif // RAWSTRUCTUREDSYSTEMMESSAGE_H
