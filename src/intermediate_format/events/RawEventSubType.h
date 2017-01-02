/**
 * intermediate_format/events/RawEventSubType.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 5def7f333053db8b0b8493555e0b7215ec91286a28a7d02398130ddc7c18ff21
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEVENTSUBTYPE_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEVENTSUBTYPE_H

#include <QDataStream>
#include <QDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace events {

enum class RawEventSubType : uint8_t {
    ANSWER_CALL,
    END_CALL,
    OFFER_CALL,
    OFFER_WEBCAM,
    START_CALL,
    ADD_TO_CONFERENCE,
    CHANGE_CONFERENCE_PICTURE,
    CHANGE_TOPIC,
    CREATE_CONFERENCE,
    DECLINE_CONFERENCE,
    JOIN_CONFERENCE,
    LEAVE_CONFERENCE,
    REJOIN_CONFERENCE,
    REMOVE_FROM_CONFERENCE,
    SET_SKYPE_CHAT_ROLE,
    JOIN_CONVERSATION,
    LEAVE_CONVERSATION,
    START_CONVERSATION,
    CANCEL_FILE_TRANSFER,
    OFFER_FILE,
    OFFER_FILE_GROUP,
    RECEIVE_FILE,
    START_FILE_TRANSFER,
    TRANSFER_FILES,
    CONTACT_DELETE,
    CONTACT_REQUEST,
    CONTACT_REQUEST_ACCEPT,
    CONNECTED,
    DISCONNECTED,
    STATUS_CHANGE,
    EE_ENCRYPTION_ANNOUNCEMENT,
    LOGGING_STARTED,
    LOGGING_STOPPED,
    WINDOW_CLOSED,
    WINDOW_OPENED,
    CHANGE_SCREEN_NAME,
    CORRUPTED_MESSAGE,
    EDITED_PREVIOUS_MESSAGE,
    MESSAGE,
    PING,
    SEND_CONTACTS,
    UNINTERPRETED,
};

QString name_for_raw_event_sub_type(RawEventSubType type);

QDebug operator<< (QDebug stream, RawEventSubType type);

QDataStream& operator>> (QDataStream& mut_stream, RawEventSubType& mut_type);
QDataStream& operator<< (QDataStream& mut_stream, RawEventSubType type);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEVENTSUBTYPE_H
