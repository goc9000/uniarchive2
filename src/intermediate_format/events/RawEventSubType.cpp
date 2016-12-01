/**
 * intermediate_format/events/RawEventSubType.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): b21e424e1acd87171af284a5c2d884682f6a7d0059ae0822d18e61dae74aa2df
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawEventSubType.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace events {

QString name_for_raw_event_sub_type(RawEventSubType type) {
    switch (type) {
        case RawEventSubType::ANSWER_CALL:
            return "AnswerCall";
        case RawEventSubType::END_CALL:
            return "EndCall";
        case RawEventSubType::OFFER_CALL:
            return "OfferCall";
        case RawEventSubType::OFFER_WEBCAM:
            return "OfferWebcam";
        case RawEventSubType::START_CALL:
            return "StartCall";
        case RawEventSubType::ADD_TO_CONFERENCE:
            return "AddToConference";
        case RawEventSubType::CHANGE_CONFERENCE_PICTURE:
            return "ChangeConferencePicture";
        case RawEventSubType::CHANGE_TOPIC:
            return "ChangeTopic";
        case RawEventSubType::CREATE_CONFERENCE:
            return "CreateConference";
        case RawEventSubType::DECLINE_CONFERENCE:
            return "DeclineConference";
        case RawEventSubType::JOIN_CONFERENCE:
            return "JoinConference";
        case RawEventSubType::LEAVE_CONFERENCE:
            return "LeaveConference";
        case RawEventSubType::REJOIN_CONFERENCE:
            return "RejoinConference";
        case RawEventSubType::REMOVE_FROM_CONFERENCE:
            return "RemoveFromConference";
        case RawEventSubType::SET_SKYPE_CHAT_ROLE:
            return "SetSkypeChatRole";
        case RawEventSubType::JOIN_CONVERSATION:
            return "JoinConversation";
        case RawEventSubType::LEAVE_CONVERSATION:
            return "LeaveConversation";
        case RawEventSubType::START_CONVERSATION:
            return "StartConversation";
        case RawEventSubType::CANCEL_FILE_TRANSFER:
            return "CancelFileTransfer";
        case RawEventSubType::OFFER_FILE:
            return "OfferFile";
        case RawEventSubType::OFFER_FILE_GROUP:
            return "OfferFileGroup";
        case RawEventSubType::RECEIVE_FILE:
            return "ReceiveFile";
        case RawEventSubType::START_FILE_TRANSFER:
            return "StartFileTransfer";
        case RawEventSubType::TRANSFER_FILES:
            return "TransferFiles";
        case RawEventSubType::CONTACT_DELETE:
            return "ContactDelete";
        case RawEventSubType::CONTACT_REQUEST:
            return "ContactRequest";
        case RawEventSubType::CONTACT_REQUEST_ACCEPT:
            return "ContactRequestAccept";
        case RawEventSubType::CONNECTED:
            return "Connected";
        case RawEventSubType::DISCONNECTED:
            return "Disconnected";
        case RawEventSubType::STATUS_CHANGE:
            return "StatusChange";
        case RawEventSubType::EE_ENCRYPTION_ANNOUNCEMENT:
            return "EEEncryptionAnnouncement";
        case RawEventSubType::LOGGING_STARTED:
            return "LoggingStarted";
        case RawEventSubType::LOGGING_STOPPED:
            return "LoggingStopped";
        case RawEventSubType::WINDOW_CLOSED:
            return "WindowClosed";
        case RawEventSubType::WINDOW_OPENED:
            return "WindowOpened";
        case RawEventSubType::CHANGE_SCREEN_NAME:
            return "ChangeScreenName";
        case RawEventSubType::CORRUPTED_MESSAGE:
            return "CorruptedMessage";
        case RawEventSubType::EDITED_PREVIOUS_MESSAGE:
            return "EditedPreviousMessage";
        case RawEventSubType::MESSAGE:
            return "Message";
        case RawEventSubType::PING:
            return "Ping";
        case RawEventSubType::SEND_CONTACTS:
            return "SendContacts";
        case RawEventSubType::UNINTERPRETED:
            return "Uninterpreted";
    }

    invariant_violation("Invalid RawEventSubType value (%lld)", (int64_t)type);
}

QDebug operator<< (QDebug stream, RawEventSubType type) {
    stream << QP(name_for_raw_event_sub_type(type));

    return stream;
}

QDataStream& operator<< (QDataStream& mut_stream, RawEventSubType type) {
    mut_stream << (uint8_t)type;

    return mut_stream;
}

}}}
