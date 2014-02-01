#include "RawConversation.h"

#include <iostream>


bool RawConversation::isNull() const
{
    return date.isNull() && myAccount.isNull();
}

void RawConversation::dump() const
{
    std::cout << qPrintable(ConversationKind_description(kind));
    std::cout << " on ";
    std::cout << qPrintable(myAccount.description());
    std::cout << " at ";
    std::cout << qPrintable(date.description());
    std::cout << " with ";

    bool first = true;
    for (const RawAccount& account : friendAccounts) {
        if (!first) {
            std::cout << ", ";
        }
        std::cout << qPrintable(account.description());
        first = false;
    }

    std::cout << ":";
    std::cout << std::endl;
}
