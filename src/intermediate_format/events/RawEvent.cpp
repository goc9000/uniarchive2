/**
 * intermediate_format/events/RawEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): efcf94ff3bbbaaead7782a6b86a8298fb887e5f9091c30042588da389f22e22d
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/events/calls/RawAnswerCallEvent.h"
#include "intermediate_format/events/calls/RawEndCallEvent.h"
#include "intermediate_format/events/calls/RawOfferCallEvent.h"
#include "intermediate_format/events/calls/RawOfferWebcamEvent.h"
#include "intermediate_format/events/calls/RawStartCallEvent.h"
#include "intermediate_format/events/conference/RawAddToConferenceEvent.h"
#include "intermediate_format/events/conference/RawChangeConferencePictureEvent.h"
#include "intermediate_format/events/conference/RawChangeTopicEvent.h"
#include "intermediate_format/events/conference/RawCreateConferenceEvent.h"
#include "intermediate_format/events/conference/RawDeclineConferenceEvent.h"
#include "intermediate_format/events/conference/RawJoinConferenceEvent.h"
#include "intermediate_format/events/conference/RawLeaveConferenceEvent.h"
#include "intermediate_format/events/conference/RawRejoinConferenceEvent.h"
#include "intermediate_format/events/conference/RawRemoveFromConferenceEvent.h"
#include "intermediate_format/events/conference/RawSetSkypeChatRoleEvent.h"
#include "intermediate_format/events/conversation/RawJoinConversationEvent.h"
#include "intermediate_format/events/conversation/RawLeaveConversationEvent.h"
#include "intermediate_format/events/conversation/RawStartConversationEvent.h"
#include "intermediate_format/events/file_transfer/RawCancelFileTransferEvent.h"
#include "intermediate_format/events/file_transfer/RawOfferFileEvent.h"
#include "intermediate_format/events/file_transfer/RawOfferFileGroupEvent.h"
#include "intermediate_format/events/file_transfer/RawReceiveFileEvent.h"
#include "intermediate_format/events/file_transfer/RawStartFileTransferEvent.h"
#include "intermediate_format/events/file_transfer/RawTransferFilesEvent.h"
#include "intermediate_format/events/friending/RawContactDeleteEvent.h"
#include "intermediate_format/events/friending/RawContactRequestAcceptEvent.h"
#include "intermediate_format/events/friending/RawContactRequestEvent.h"
#include "intermediate_format/events/status/RawConnectedEvent.h"
#include "intermediate_format/events/status/RawDisconnectedEvent.h"
#include "intermediate_format/events/status/RawStatusChangeEvent.h"
#include "intermediate_format/events/system/RawEEEncryptionAnnouncementEvent.h"
#include "intermediate_format/events/system/RawLoggingStartedEvent.h"
#include "intermediate_format/events/system/RawLoggingStoppedEvent.h"
#include "intermediate_format/events/system/RawWindowClosedEvent.h"
#include "intermediate_format/events/system/RawWindowOpenedEvent.h"
#include "intermediate_format/events/RawChangeScreenNameEvent.h"
#include "intermediate_format/events/RawCorruptedMessageEvent.h"
#include "intermediate_format/events/RawEditedPreviousMessageEvent.h"
#include "intermediate_format/events/RawMessageEvent.h"
#include "intermediate_format/events/RawPingEvent.h"
#include "intermediate_format/events/RawSendContactsEvent.h"
#include "intermediate_format/events/RawUninterpretedEvent.h"
#include "utils/qt/debug_extras.h"
#include "utils/qt/shortcuts.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

RawEvent::RawEvent(IMM(ApparentTime) timestamp, uint32_t index)
  : StandardPolymorphic(), timestamp(timestamp), indexInConversation(index) {
}

QString RawEvent::eventName() const {
    return name_for_raw_event_sub_type(subType());
}

CEDE(RawEvent) RawEvent::deserializeFromStream(QDataStream& mut_stream) {
    RawEventSubType subtype = must_deserialize(mut_stream, RawEventSubType);

    switch (subtype) {
        case RawEventSubType::ANSWER_CALL:
            return RawAnswerCallEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::END_CALL:
            return RawEndCallEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::OFFER_CALL:
            return RawOfferCallEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::OFFER_WEBCAM:
            return RawOfferWebcamEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::START_CALL:
            return RawStartCallEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::ADD_TO_CONFERENCE:
            return RawAddToConferenceEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::CHANGE_CONFERENCE_PICTURE:
            return RawChangeConferencePictureEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::CHANGE_TOPIC:
            return RawChangeTopicEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::CREATE_CONFERENCE:
            return RawCreateConferenceEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::DECLINE_CONFERENCE:
            return RawDeclineConferenceEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::JOIN_CONFERENCE:
            return RawJoinConferenceEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::LEAVE_CONFERENCE:
            return RawLeaveConferenceEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::REJOIN_CONFERENCE:
            return RawRejoinConferenceEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::REMOVE_FROM_CONFERENCE:
            return RawRemoveFromConferenceEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::SET_SKYPE_CHAT_ROLE:
            return RawSetSkypeChatRoleEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::JOIN_CONVERSATION:
            return RawJoinConversationEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::LEAVE_CONVERSATION:
            return RawLeaveConversationEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::START_CONVERSATION:
            return RawStartConversationEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::CANCEL_FILE_TRANSFER:
            return RawCancelFileTransferEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::OFFER_FILE:
            return RawOfferFileEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::OFFER_FILE_GROUP:
            return RawOfferFileGroupEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::RECEIVE_FILE:
            return RawReceiveFileEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::START_FILE_TRANSFER:
            return RawStartFileTransferEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::TRANSFER_FILES:
            return RawTransferFilesEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::CONTACT_DELETE:
            return RawContactDeleteEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::CONTACT_REQUEST:
            return RawContactRequestEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::CONTACT_REQUEST_ACCEPT:
            return RawContactRequestAcceptEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::CONNECTED:
            return RawConnectedEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::DISCONNECTED:
            return RawDisconnectedEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::STATUS_CHANGE:
            return RawStatusChangeEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::EE_ENCRYPTION_ANNOUNCEMENT:
            return RawEEEncryptionAnnouncementEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::LOGGING_STARTED:
            return RawLoggingStartedEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::LOGGING_STOPPED:
            return RawLoggingStoppedEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::WINDOW_CLOSED:
            return RawWindowClosedEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::WINDOW_OPENED:
            return RawWindowOpenedEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::CHANGE_SCREEN_NAME:
            return RawChangeScreenNameEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::CORRUPTED_MESSAGE:
            return RawCorruptedMessageEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::EDITED_PREVIOUS_MESSAGE:
            return RawEditedPreviousMessageEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::MESSAGE:
            return RawMessageEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::PING:
            return RawPingEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::SEND_CONTACTS:
            return RawSendContactsEvent::deserializeFromStream(mut_stream, true);
        case RawEventSubType::UNINTERPRETED:
            return RawUninterpretedEvent::deserializeFromStream(mut_stream, true);
    }

    invariant_violation("Invalid deserialized RawEvent subtype (code: %d)", (int)subtype);
}

void RawEvent::serializeToStreamImpl(QDataStream& mut_stream) const {
    mut_stream << timestamp << indexInConversation << skypeGUID;

    serializeDetailsToStream(mut_stream);
}

void RawEvent::serializeDetailsToStream(QDataStream& UNUSED mut_stream) const {
    // Override this in children
}

void RawEvent::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace();

    stream << "#" << indexInConversation << " ";
    stream << "[" << timestamp << "] ";

    stream << QP(eventName());
    writeDetailsToDebugStream(stream);

    if (skypeGUID) {
        stream << " skype_guid=" << skypeGUID;
    }
}

void RawEvent::writeDetailsToDebugStream(QDebug UNUSED stream) const {
}

}}}
