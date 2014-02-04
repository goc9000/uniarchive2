#include "constants.h"


QString IMProtocol_description(IMProtocol value)
{
    switch (value) {
    case IMProtocol::INVALID: return "INVALID!";
    case IMProtocol::YAHOO:   return "Yahoo";
    case IMProtocol::XMPP:    return "XMPP";
    case IMProtocol::MSN:     return "MSN";
    }

    return QString::number((int)value) + "?";
}

QString ConversationKind_description(ConversationKind value)
{
    switch (value) {
    case ConversationKind::INVALID:    return "INVALID!";
    case ConversationKind::UNKNOWN:    return "Conversation";
    case ConversationKind::ONE_ON_ONE: return "One-on-one";
    case ConversationKind::CONFERENCE: return "Conference";
    }

    return QString::number((int)value) + "?";
}

QString SystemMessagePredicate_description(SystemMessagePredicate value)
{
    switch (value) {
    case SystemMessagePredicate::INVALID:
        return "INVALID!";
    case SystemMessagePredicate::LOGGED_IN:
        return "LoggedIn";
    case SystemMessagePredicate::LOGGED_OUT:
        return "LoggedOut";
    }

    return QString::number((int)value) + "?";
}

