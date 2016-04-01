#ifndef RAWSTRUCTUREDSYSTEMMESSAGE_H
#define RAWSTRUCTUREDSYSTEMMESSAGE_H

#include "model/raw_conversation/messages/RawSystemMessage.h"
#include "model/raw_conversation/RawSpeaker.h"
#include "model/raw_conversation/RawTransferredFileRef.h"
#include "model/constants.h"


class RawStructuredSystemMessage : public RawSystemMessage
{
public:
    class Param
    {
    public:
        enum Type { EMPTY, SPEAKER, PRESENCE, FILE, COUNT, SEND_ERROR };

        Param() : tag(EMPTY) {}
        Param(RawSpeaker* speaker)
            : tag(SPEAKER), speaker(speaker) {}
        Param(PresenceState presence)
            : tag(PRESENCE), presence(presence) {}
        Param(RawTransferredFileRef* file)
            : tag(FILE), file(file) {}
        Param(unsigned int count)
            : tag(COUNT), count(count) {}
        Param(MessageSendFailedReason sendError)
            : tag(SEND_ERROR), sendError(sendError) {}

        bool isNull() const;
        Type type() const;

        QString description() const;
    protected:
        Type tag;
        union {
            RawSpeaker* speaker;
            PresenceState presence;
            RawTransferredFileRef* file;
            unsigned int count;
            MessageSendFailedReason sendError;
        };
    };

    RawStructuredSystemMessage(TimeStamp date, RawMessage::Flags flags,
                               Param subject, SystemMessagePredicate predicate,
                               Param object=Param(), Param aux=Param())
        : RawSystemMessage(date, flags), subject(subject),
          predicate(predicate), object(object), aux(aux) {}

    Param subject;
    SystemMessagePredicate predicate;
    Param object;
    Param aux;
protected:
    QString _sysMessageDescription() const;
};

#endif // RAWSTRUCTUREDSYSTEMMESSAGE_H