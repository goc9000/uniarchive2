#ifndef RAWCONVERSATIONSOURCE_H
#define RAWCONVERSATIONSOURCE_H

#include <QString>
#include <QList>

#include "model/raw_conversation/RawConversation.h"


class RawConversationSource
{
public:
    virtual QList<RawConversation> rawConversations() = 0;

    static RawConversationSource* fromSpecification(QString spec);
protected:
    RawConversationSource() {}
};

#endif // RAWCONVERSATIONSOURCE_H
