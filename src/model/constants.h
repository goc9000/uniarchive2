#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>


enum class IMProtocol
{
    INVALID = 0,
    YAHOO   = 1,
    XMPP    = 2,
    MSN     = 3
};

QString IMProtocol_description(IMProtocol value);


enum class ConversationKind
{
    INVALID    = 0,
    UNKNOWN    = 1,
    ONE_ON_ONE = 2,
    CONFERENCE = 3
};

QString ConversationKind_description(ConversationKind value);


enum class SystemMessagePredicate
{
    INVALID                   = 0,
    LOGGED_IN                 = 1,
    LOGGED_OUT                = 2
};

QString SystemMessagePredicate_description(SystemMessagePredicate value);

#endif // CONSTANTS_H
