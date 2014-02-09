#ifndef PIDGINSYSTEMMESSAGEPARSER_H
#define PIDGINSYSTEMMESSAGEPARSER_H

#include "model/raw_conversation/RawConversation.h"
#include "model/raw_conversation/messages/RawSystemMessage.h"


class PidginSystemMessageParser
{
public:
    static RawMessageUqPtr parseSystemMessage(
        TimeStamp messageDate, bool isOffline, QString messageText,
        RawConversation& conversation);
};

#endif // PIDGINSYSTEMMESSAGEPARSER_H
