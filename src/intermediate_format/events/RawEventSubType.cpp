/**
 * intermediate_format/events/RawEventSubType.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): d6576ea429ef04e05778a390f078133109d4907a99a6b780eabff483a3b503fe
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

#include <QMap>
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

QString symbol_for_raw_event_sub_type(RawEventSubType type) {
    switch (type) {
        case RawEventSubType::ANSWER_CALL:
            return "answer_call";
        case RawEventSubType::END_CALL:
            return "end_call";
        case RawEventSubType::OFFER_CALL:
            return "offer_call";
        case RawEventSubType::OFFER_WEBCAM:
            return "offer_webcam";
        case RawEventSubType::START_CALL:
            return "start_call";
        case RawEventSubType::ADD_TO_CONFERENCE:
            return "add_to_conference";
        case RawEventSubType::CHANGE_CONFERENCE_PICTURE:
            return "change_conference_picture";
        case RawEventSubType::CHANGE_TOPIC:
            return "change_topic";
        case RawEventSubType::CREATE_CONFERENCE:
            return "create_conference";
        case RawEventSubType::DECLINE_CONFERENCE:
            return "decline_conference";
        case RawEventSubType::JOIN_CONFERENCE:
            return "join_conference";
        case RawEventSubType::LEAVE_CONFERENCE:
            return "leave_conference";
        case RawEventSubType::REJOIN_CONFERENCE:
            return "rejoin_conference";
        case RawEventSubType::REMOVE_FROM_CONFERENCE:
            return "remove_from_conference";
        case RawEventSubType::SET_SKYPE_CHAT_ROLE:
            return "set_skype_chat_role";
        case RawEventSubType::JOIN_CONVERSATION:
            return "join_conversation";
        case RawEventSubType::LEAVE_CONVERSATION:
            return "leave_conversation";
        case RawEventSubType::START_CONVERSATION:
            return "start_conversation";
        case RawEventSubType::CANCEL_FILE_TRANSFER:
            return "cancel_file_transfer";
        case RawEventSubType::OFFER_FILE:
            return "offer_file";
        case RawEventSubType::OFFER_FILE_GROUP:
            return "offer_file_group";
        case RawEventSubType::RECEIVE_FILE:
            return "receive_file";
        case RawEventSubType::START_FILE_TRANSFER:
            return "start_file_transfer";
        case RawEventSubType::TRANSFER_FILES:
            return "transfer_files";
        case RawEventSubType::CONTACT_DELETE:
            return "contact_delete";
        case RawEventSubType::CONTACT_REQUEST:
            return "contact_request";
        case RawEventSubType::CONTACT_REQUEST_ACCEPT:
            return "contact_request_accept";
        case RawEventSubType::CONNECTED:
            return "connected";
        case RawEventSubType::DISCONNECTED:
            return "disconnected";
        case RawEventSubType::STATUS_CHANGE:
            return "status_change";
        case RawEventSubType::EE_ENCRYPTION_ANNOUNCEMENT:
            return "ee_encryption_announcement";
        case RawEventSubType::LOGGING_STARTED:
            return "logging_started";
        case RawEventSubType::LOGGING_STOPPED:
            return "logging_stopped";
        case RawEventSubType::WINDOW_CLOSED:
            return "window_closed";
        case RawEventSubType::WINDOW_OPENED:
            return "window_opened";
        case RawEventSubType::CHANGE_SCREEN_NAME:
            return "change_screen_name";
        case RawEventSubType::CORRUPTED_MESSAGE:
            return "corrupted_message";
        case RawEventSubType::EDITED_PREVIOUS_MESSAGE:
            return "edited_previous_message";
        case RawEventSubType::MESSAGE:
            return "message";
        case RawEventSubType::PING:
            return "ping";
        case RawEventSubType::SEND_CONTACTS:
            return "send_contacts";
        case RawEventSubType::UNINTERPRETED:
            return "uninterpreted";
    }

    invariant_violation("Invalid RawEventSubType value (%lld)", (int64_t)type);
}

RawEventSubType raw_event_sub_type_from_symbol(IMM(QString) symbol) {
    const static QMap<QString, RawEventSubType> MAP = {
        { "answer_call",                RawEventSubType::ANSWER_CALL },
        { "end_call",                   RawEventSubType::END_CALL },
        { "offer_call",                 RawEventSubType::OFFER_CALL },
        { "offer_webcam",               RawEventSubType::OFFER_WEBCAM },
        { "start_call",                 RawEventSubType::START_CALL },
        { "add_to_conference",          RawEventSubType::ADD_TO_CONFERENCE },
        { "change_conference_picture",  RawEventSubType::CHANGE_CONFERENCE_PICTURE },
        { "change_topic",               RawEventSubType::CHANGE_TOPIC },
        { "create_conference",          RawEventSubType::CREATE_CONFERENCE },
        { "decline_conference",         RawEventSubType::DECLINE_CONFERENCE },
        { "join_conference",            RawEventSubType::JOIN_CONFERENCE },
        { "leave_conference",           RawEventSubType::LEAVE_CONFERENCE },
        { "rejoin_conference",          RawEventSubType::REJOIN_CONFERENCE },
        { "remove_from_conference",     RawEventSubType::REMOVE_FROM_CONFERENCE },
        { "set_skype_chat_role",        RawEventSubType::SET_SKYPE_CHAT_ROLE },
        { "join_conversation",          RawEventSubType::JOIN_CONVERSATION },
        { "leave_conversation",         RawEventSubType::LEAVE_CONVERSATION },
        { "start_conversation",         RawEventSubType::START_CONVERSATION },
        { "cancel_file_transfer",       RawEventSubType::CANCEL_FILE_TRANSFER },
        { "offer_file",                 RawEventSubType::OFFER_FILE },
        { "offer_file_group",           RawEventSubType::OFFER_FILE_GROUP },
        { "receive_file",               RawEventSubType::RECEIVE_FILE },
        { "start_file_transfer",        RawEventSubType::START_FILE_TRANSFER },
        { "transfer_files",             RawEventSubType::TRANSFER_FILES },
        { "contact_delete",             RawEventSubType::CONTACT_DELETE },
        { "contact_request",            RawEventSubType::CONTACT_REQUEST },
        { "contact_request_accept",     RawEventSubType::CONTACT_REQUEST_ACCEPT },
        { "connected",                  RawEventSubType::CONNECTED },
        { "disconnected",               RawEventSubType::DISCONNECTED },
        { "status_change",              RawEventSubType::STATUS_CHANGE },
        { "ee_encryption_announcement", RawEventSubType::EE_ENCRYPTION_ANNOUNCEMENT },
        { "logging_started",            RawEventSubType::LOGGING_STARTED },
        { "logging_stopped",            RawEventSubType::LOGGING_STOPPED },
        { "window_closed",              RawEventSubType::WINDOW_CLOSED },
        { "window_opened",              RawEventSubType::WINDOW_OPENED },
        { "change_screen_name",         RawEventSubType::CHANGE_SCREEN_NAME },
        { "corrupted_message",          RawEventSubType::CORRUPTED_MESSAGE },
        { "edited_previous_message",    RawEventSubType::EDITED_PREVIOUS_MESSAGE },
        { "message",                    RawEventSubType::MESSAGE },
        { "ping",                       RawEventSubType::PING },
        { "send_contacts",              RawEventSubType::SEND_CONTACTS },
        { "uninterpreted",              RawEventSubType::UNINTERPRETED },
    };

    if (MAP.contains(symbol)) {
        return MAP[symbol];
    }

    invariant_violation("Invalid RawEventSubType symbol ('%s')", QP(symbol));
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
