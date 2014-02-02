#include "RawConversation.h"

#include "utils/utils.h"

#include <iostream>


RawConversation::RawConversation(RawConversation&& other)
    : date(other.date), kind(other.kind), myAccount(std::move(other.myAccount))
{
    appendByMoving(friendAccounts, other.friendAccounts);
}

bool RawConversation::isNull() const
{
    return date.isNull() && !myAccount;
}

void RawConversation::dump() const
{
    if (isNull())  {
        std::cout << "<null conversation>" << std::endl;
        return;
    }

    std::cout << qPrintable(ConversationKind_description(kind));
    std::cout << " on ";
    std::cout << qPrintable(myAccount->description());
    std::cout << " at ";
    std::cout << qPrintable(date.description());
    std::cout << " with ";

    bool first = true;
    for (const RawAccountUqPtr& account : friendAccounts) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << qPrintable(account->description());

        first = false;
    }

    std::cout << ":";
    std::cout << std::endl;
}
