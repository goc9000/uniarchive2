#ifndef RAWCONVERSATIONSOURCE_H
#define RAWCONVERSATIONSOURCE_H

#include <memory>
#include <vector>

#include <QString>

#include "model/raw_conversation/RawConversation.h"


class RawConversationSource;
typedef std::unique_ptr<RawConversationSource> RawConversationSourceUqPtr;


class RawConversationSource
{
public:
    virtual ~RawConversationSource();

    virtual std::vector<RawConversation> rawConversations() = 0;

    static RawConversationSourceUqPtr fromSpecification(QString spec);
protected:
    RawConversationSource();
};

#endif // RAWCONVERSATIONSOURCE_H
