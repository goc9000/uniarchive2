#ifndef RAWCONVERSATION_H
#define RAWCONVERSATION_H

#include <vector>

#include "model/time/TimeStamp.h"
#include "model/raw_conversation/RawAccount.h"
#include "model/constants.h"


class RawConversation
{
public:
    RawConversation()
        : date(), kind(ConversationKind::INVALID), myAccount(nullptr) {}
    RawConversation(TimeStamp date, ConversationKind kind, RawAccount myAccount)
        : date(date), kind(kind), myAccount(new RawAccount(myAccount)) {}
    RawConversation(RawConversation&& other);

    TimeStamp date;
    ConversationKind kind;

    RawAccountUqPtr myAccount;
    std::vector<RawAccountUqPtr> friendAccounts;

    bool isNull() const;

    void dump() const;
};

#endif // RAWCONVERSATION_H
