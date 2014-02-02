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

RawAccount* RawConversation::getAccount(QString id, IMProtocol protocol) const
{
    if ((myAccount->id == id) && (myAccount->protocol == protocol)) {
        return myAccount.get();
    }

    for (const RawAccountUqPtr& frAccount : friendAccounts) {
        if ((frAccount->id == id) && (frAccount->protocol == protocol)) {
            return frAccount.get();
        }
    }

    return nullptr;
}

RawAccount* RawConversation::addFriendAccount(QString id, IMProtocol protocol)
{
    RawAccount* account = getAccount(id, protocol);
    if (account) {
        return account;
    }

    account = new RawAccount(id, protocol);
    friendAccounts.emplace_back(RawAccountUqPtr(account));

    return account;
}
