/**
 * intermediate_format/events/RawEventSubType.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 8a0e8887d96a558103a7fd9afda5b8cd50932bd83804a15b30a3e1da34655ebf
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawEventSubType.h"
#include "utils/qt/shortcuts.h"
#include "utils/serialization/deserialization_helpers.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

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

QDataStream& operator>> (QDataStream& mut_stream, RawEventSubType& mut_type) {
    uint8_t code = must_deserialize(mut_stream, uint8_t);

    switch ((RawEventSubType)code) {
        case RawEventSubType::ANSWER_CALL:
        case RawEventSubType::END_CALL:
        case RawEventSubType::OFFER_CALL:
        case RawEventSubType::OFFER_WEBCAM:
        case RawEventSubType::START_CALL:
        case RawEventSubType::ADD_TO_CONFERENCE:
        case RawEventSubType::CHANGE_CONFERENCE_PICTURE:
        case RawEventSubType::CHANGE_TOPIC:
        case RawEventSubType::CREATE_CONFERENCE:
        case RawEventSubType::DECLINE_CONFERENCE:
        case RawEventSubType::JOIN_CONFERENCE:
        case RawEventSubType::LEAVE_CONFERENCE:
        case RawEventSubType::REJOIN_CONFERENCE:
        case RawEventSubType::REMOVE_FROM_CONFERENCE:
        case RawEventSubType::SET_SKYPE_CHAT_ROLE:
        case RawEventSubType::JOIN_CONVERSATION:
        case RawEventSubType::LEAVE_CONVERSATION:
        case RawEventSubType::START_CONVERSATION:
        case RawEventSubType::CANCEL_FILE_TRANSFER:
        case RawEventSubType::OFFER_FILE:
        case RawEventSubType::OFFER_FILE_GROUP:
        case RawEventSubType::RECEIVE_FILE:
        case RawEventSubType::START_FILE_TRANSFER:
        case RawEventSubType::TRANSFER_FILES:
        case RawEventSubType::CONTACT_DELETE:
        case RawEventSubType::CONTACT_REQUEST:
        case RawEventSubType::CONTACT_REQUEST_ACCEPT:
        case RawEventSubType::CONNECTED:
        case RawEventSubType::DISCONNECTED:
        case RawEventSubType::STATUS_CHANGE:
        case RawEventSubType::EE_ENCRYPTION_ANNOUNCEMENT:
        case RawEventSubType::LOGGING_STARTED:
        case RawEventSubType::LOGGING_STOPPED:
        case RawEventSubType::WINDOW_CLOSED:
        case RawEventSubType::WINDOW_OPENED:
        case RawEventSubType::CHANGE_SCREEN_NAME:
        case RawEventSubType::CORRUPTED_MESSAGE:
        case RawEventSubType::EDITED_PREVIOUS_MESSAGE:
        case RawEventSubType::MESSAGE:
        case RawEventSubType::PING:
        case RawEventSubType::SEND_CONTACTS:
        case RawEventSubType::UNINTERPRETED:
            mut_type = (RawEventSubType)code;
            break;
        default:
            invariant_violation("Invalid deserialized RawEventSubType value (%lld)", (int64_t)code);
    }

    return mut_stream;
}

QDataStream& operator<< (QDataStream& mut_stream, RawEventSubType type) {
    mut_stream << (uint8_t)type;

    return mut_stream;
}

}}}
