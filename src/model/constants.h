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
IMProtocol IMProtocol_parse(QString protoText);
IMProtocol IMProtocol_parseOrFail(QString protoText);


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
    LOGGED_OUT                = 2,
    CHANGED_STATE             = 3,
    REVERTED_STATE            = 4
};

QString SystemMessagePredicate_description(SystemMessagePredicate value);


enum class PresenceState
{
    INVALID   = 0,
    AVAILABLE = 1,
    IDLE      = 2,
    BUSY      = 3,
    AWAY      = 4
};

QString PresenceState_description(PresenceState value);
PresenceState PresenceState_parse(QString stateText);
PresenceState PresenceState_parseOrFail(QString stateText);

#endif // CONSTANTS_H
