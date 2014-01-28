#ifndef RAWCONVERSATIONSOURCE_H
#define RAWCONVERSATIONSOURCE_H

#include <vector>

#include <QString>

#include "model/raw_conversation/RawConversation.h"


class RawConversationSource
{
public:
    virtual ~RawConversationSource();

    virtual std::vector<RawConversation> rawConversations() = 0;

    static RawConversationSource* fromSpecification(QString spec);
protected:
    RawConversationSource();
};

#endif // RAWCONVERSATIONSOURCE_H
