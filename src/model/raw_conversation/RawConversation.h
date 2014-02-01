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
        : date(), kind(ConversationKind::INVALID), myAccount() {}
    RawConversation(TimeStamp date, ConversationKind kind,
                    RawAccount myAccount)
        : date(date), kind(kind), myAccount(myAccount) {}

    TimeStamp date;
    ConversationKind kind;

    RawAccount myAccount;
    std::vector<RawAccount> friendAccounts;

    bool isNull() const;

    void dump() const;
};

#endif // RAWCONVERSATION_H
