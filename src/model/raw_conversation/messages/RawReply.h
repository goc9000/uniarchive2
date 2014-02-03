#ifndef RAWREPLY_H
#define RAWREPLY_H

#include "model/raw_conversation/messages/RawMessage.h"
#include "model/raw_conversation/RawSpeaker.h"


class RawReply : public RawMessage
{
public:
    RawReply(TimeStamp date, bool isOffline, RawSpeaker* speaker,
             QString content)
        : RawMessage(date, isOffline), speaker(speaker), content(content) {}

    RawSpeaker* speaker;
    QString content;
protected:
    QString _bodyDescription() const;
};

#endif // RAWREPLY_H
