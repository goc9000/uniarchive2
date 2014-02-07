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
    INVALID                        = 0,
    LOGGED_IN                      = 1,
    LOGGED_OUT                     = 2,
    CHANGED_STATE                  = 3,
    REVERTED_STATE                 = 4,
    CHANGED_ALIAS                  = 5,
    SENT_BUZZ                      = 10,
    JOINED_CONFERENCE              = 20,
    LEFT_CONFERENCE                = 21,
    OFFERED_FILE                   = 30,
    OFFERED_FILE_GROUP             = 31,
    FILE_TRANSFER_STARTED          = 32,
    FILE_TRANSFER_COMPLETE         = 33,
    CANCELLED_FILE_TRANSFER        = 34,
    MESSAGE_SEND_FAILED            = 50,
    SENT_WEBCAM_INVITE_UNSUPPORTED = 100
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


enum class MessageSendFailedReason
{
    INVALID           = 0,
    MESSAGE_TOO_LARGE = 1,
    PEER_IS_OFFLINE   = 2,
    CONNECTION_ERROR  = 3
};

QString MessageSendFailedReason_description(MessageSendFailedReason value);

#endif // CONSTANTS_H
