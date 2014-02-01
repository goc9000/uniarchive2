#include "RawConversation.h"

bool RawConversation::isNull() const
{
    return date.isNull() && myAccount.isNull();
}
