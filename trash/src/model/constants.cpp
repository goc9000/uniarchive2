#include "constants.h"

#include "utils/fail.h"


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

IMProtocol IMProtocol_parse(QString protoText)
{
    protoText = protoText.trimmed().toLower();

    if (protoText == "yahoo") {
        return IMProtocol::YAHOO;
    } else if (protoText == "jabber") {
        return IMProtocol::XMPP;
    } else if (protoText == "msn") {
        return IMProtocol::MSN;
    }

    return IMProtocol::INVALID;
}

IMProtocol IMProtocol_parseOrFail(QString protoText)
{
    IMProtocol value = IMProtocol_parse(protoText);

    if (value == IMProtocol::INVALID) {
        fail("Unrecognized protocol string '%s'", qPrintable(protoText));
    }

    return value;
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
    case SystemMessagePredicate::CHANGED_STATE:
        return "ChangedState";
    case SystemMessagePredicate::REVERTED_STATE:
        return "RevertedState";
    case SystemMessagePredicate::CHANGED_ALIAS:
        return "ChangedAlias";
    case SystemMessagePredicate::SENT_BUZZ:
        return "SentBuzz";
    case SystemMessagePredicate::JOINED_CONFERENCE:
        return "JoinedConference";
    case SystemMessagePredicate::LEFT_CONFERENCE:
        return "LeftConference";
    case SystemMessagePredicate::OFFERED_FILE:
        return "OfferedFile";
    case SystemMessagePredicate::OFFERED_FILE_GROUP:
        return "OfferedFileGroup";
    case SystemMessagePredicate::FILE_TRANSFER_STARTED:
        return "FileTransferStarted";
    case SystemMessagePredicate::FILE_TRANSFER_COMPLETE:
        return "FileTransferComplete";
    case SystemMessagePredicate::CANCELLED_FILE_TRANSFER:
        return "CancelledFileTransfer";
    case SystemMessagePredicate::MESSAGE_SEND_FAILED:
        return "MessageSendFailed";
    case SystemMessagePredicate::SENT_WEBCAM_INVITE_UNSUPPORTED:
        return "SentWebcamInviteUnsupported";
    }

    return QString::number((int)value) + "?";
}


QString PresenceState_description(PresenceState value)
{
    switch (value) {
    case PresenceState::INVALID:   return "INVALID!";
    case PresenceState::AVAILABLE: return "Available";
    case PresenceState::IDLE:      return "Idle";
    case PresenceState::BUSY:      return "Busy";
    case PresenceState::AWAY:      return "Away";
    }

    return QString::number((int)value) + "?";
}

PresenceState PresenceState_parse(QString stateText)
{
    stateText = stateText.trimmed().toLower();

    if (stateText == "available") {
        return PresenceState::AVAILABLE;
    } else if (stateText == "idle") {
        return PresenceState::IDLE;
    } else if (stateText == "busy") {
        return PresenceState::BUSY;
    } else if (stateText == "away") {
        return PresenceState::AWAY;
    }

    return PresenceState::INVALID;
}

PresenceState PresenceState_parseOrFail(QString stateText)
{
    PresenceState value = PresenceState_parse(stateText);

    if (value == PresenceState::INVALID) {
        fail("Unrecognized presence state string '%s'", qPrintable(stateText));
    }

    return value;
}


QString MessageSendFailedReason_description(MessageSendFailedReason value)
{
    switch (value) {
    case MessageSendFailedReason::INVALID:
        return "INVALID!";
    case MessageSendFailedReason::MESSAGE_TOO_LARGE:
        return "Message too large";
    case MessageSendFailedReason::PEER_IS_OFFLINE:
        return "Peer is offline";
    case MessageSendFailedReason::CONNECTION_ERROR:
        return "Connection error";
    }

    return QString::number((int)value) + "?";
}