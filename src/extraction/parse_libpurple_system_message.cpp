/**
 * extraction/parse_libpurple_system_message.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "extraction/parse_libpurple_system_message.h"
#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/errors/OfferWebcamFailReason.h"
#include "intermediate_format/errors/PingFailReason.h"
#include "intermediate_format/errors/SendMessageFailReason.h"
#include "intermediate_format/events/calls/RawOfferWebcamEvent.h"
#include "intermediate_format/events/conference/RawJoinConferenceEvent.h"
#include "intermediate_format/events/conference/RawLeaveConferenceEvent.h"
#include "intermediate_format/events/conversation/RawLeaveConversationEvent.h"
#include "intermediate_format/events/file_transfer/RawCancelFileTransferEvent.h"
#include "intermediate_format/events/file_transfer/RawOfferFileEvent.h"
#include "intermediate_format/events/file_transfer/RawOfferFileGroupEvent.h"
#include "intermediate_format/events/file_transfer/RawReceiveFileEvent.h"
#include "intermediate_format/events/file_transfer/RawStartFileTransferEvent.h"
#include "intermediate_format/events/status/RawConnectedEvent.h"
#include "intermediate_format/events/status/RawDisconnectedEvent.h"
#include "intermediate_format/events/status/RawStatusChangeEvent.h"
#include "intermediate_format/events/system/RawLoggingStartedEvent.h"
#include "intermediate_format/events/system/RawLoggingStoppedEvent.h"
#include "intermediate_format/events/RawChangeScreenNameEvent.h"
#include "intermediate_format/events/RawMessageEvent.h"
#include "intermediate_format/events/RawPingEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/subjects/ImplicitSubject.h"
#include "intermediate_format/subjects/FullySpecifiedSubject.h"
#include "intermediate_format/subjects/SubjectGivenAsScreenName.h"
#include "intermediate_format/RawTransferredFile.h"
#include "protocols/parse_account_generic.h"
#include "utils/html/entities.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace extraction {

using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::utils::html;

static QString strip_subject_suffix(IMM(QString) subject);
static RawTransferredFile parse_file(IMM(QString) filename_html, bool is_html);


CEDE(RawEvent) parse_libpurple_system_message(
    unsigned int event_index,
    IMM(ApparentTime) event_time,
    IMM(QString) content,
    bool is_html,
    IMProtocol protocol
) {
    QREGEX_MATCH_CI(
        match,
        "^("\
        "((?<offer_grp_who>.+) is trying to send you a group of (?<n_files_group>\\d+) files[.])|"\
        "((?<offer_who>.+) is offering to send file (?<offer_file>.+))|"\
        "(Starting transfer of (?<xfer_file>.+) from (?<xfer_from>.+))|"\
        "(((?<cancel_you>You)|(?<cancel_who>.+)) cancell?ed the transfer of (?<cancel_file>.+))|"\
        "(?<cancel_undef>File transfer cancelled)|"\
        "(Transfer of file (?<recv_file>.+) complete)|"\
        "(Offering to send (?<you_offer_file>.+) to (?<you_offer_to>.+))|"\
        "(Buzzing (?<buzz_to>.+)[.][.][.])|"\
        "(?<you_buzz>: Buzz!!)|"\
        "(Unable to buzz, because (?<buzz_to_fail>.+) does not support it or does not wish to receive buzzes now[.])|"\
        "((?<buzz_from>.+) (has buzzed you|just sent you a Buzz)!)|"\
        "((?<rename_old>.+) is now known as (?<rename_new>.+)[.])|"\
        "((?<leave_11_who>.+) has left the conversation[.])|"\
        "((?<join_conf_who>.+) entered the room[.])|"\
        "((?<leave_conf_who>.+) left the room[.])|"\
        "((?<sign_on_who>.+) (has signed on|logged in)[.])|"\
        "((?<sign_off_who>.+) (has signed off|logged out)[.])|"\
        "((?<away_who>.+) has gone away[.])|"\
        "((?<idle_who>.+) has become idle[.])|"\
        "((?<avail_who>.+) is no longer (idle|away)[.])|"\
        "(?<msg_too_large>Unable to send message: The message is too large[.])|"\
        "(?<msg_not_sent>Message could not be sent because "\
          "((?<fail_user_offline>the user is offline)|(?<fail_conn_err>a connection error occurred))"\
          ":\\s*(?<unsent_msg>.*)"\
        ")|"\
        "((?<unsup_webcam_from>.+) has sent you a webcam invite, which is not yet supported[.])|"\
        "(?<log_started>Logging started[.] Future messages in this conversation will be logged[.])|"\
        "(?<log_stopped>Logging stopped[.] Future messages in this conversation will not be logged[.])"\
        ")(<br/?>)?$",
        content.trimmed()
    );

    if (match.capturedLength("offer_grp_who")) {
        return make_unique<RawOfferFileGroupEvent>(
            event_time,
            event_index,
            parse_libpurple_subject(match.captured("offer_grp_who"), protocol, is_html),
            match.captured("n_files_group").toUInt()
        );
    } else if (match.capturedLength("offer_who")) {
        return make_unique<RawOfferFileEvent>(
            event_time,
            event_index,
            parse_libpurple_subject(match.captured("offer_who"), protocol, is_html),
            parse_file(match.captured("offer_file"), is_html)
        );
    } else if (match.capturedLength("xfer_file")) {
        unique_ptr<RawEvent> xfer_event = make_unique<RawStartFileTransferEvent>(
            event_time,
            event_index,
            parse_file(match.captured("xfer_file"), is_html)
        );
        xfer_event->as<RawStartFileTransferEvent>()->sender =
            parse_libpurple_subject(match.captured("xfer_from"), protocol, is_html);

        return xfer_event;
    } else if (match.capturedLength("cancel_file")) {
        unique_ptr<RawEvent> xfer_event = make_unique<RawCancelFileTransferEvent>(
            event_time,
            event_index,
            parse_file(match.captured("cancel_file"), is_html)
        );
        xfer_event->as<RawCancelFileTransferEvent>()->actor =
            match.capturedLength("cancel_you")
                ? make_unique<ImplicitSubject>(ImplicitSubject::Kind::IDENTITY)
                : parse_libpurple_subject(match.captured("cancel_who"), protocol, is_html);

        return xfer_event;
    } else if (match.capturedLength("cancel_undef")) {
        return make_unique<RawCancelFileTransferEvent>(event_time, event_index);
    } else if (match.capturedLength("recv_file")) {
        return make_unique<RawReceiveFileEvent>(
            event_time,
            event_index,
            make_unique<ImplicitSubject>(ImplicitSubject::Kind::FILE_RECEIVER),
            parse_file(match.captured("recv_file"), is_html)
        );

    } else if (match.capturedLength("you_offer_file")) {
        unique_ptr<RawEvent> offer_event = make_unique<RawOfferFileEvent>(
            event_time,
            event_index,
            make_unique<ImplicitSubject>(ImplicitSubject::Kind::IDENTITY),
            parse_file(match.captured("you_offer_file"), is_html)
        );
        offer_event->as<RawOfferFileEvent>()->recipient =
            parse_libpurple_subject(match.captured("you_offer_to"), protocol, is_html);

        return offer_event;
    } else if (match.capturedLength("you_buzz") || match.capturedLength("buzz_to")) {
        unique_ptr<RawEvent> ping_event = make_unique<RawPingEvent>(
            event_time,
            event_index,
            make_unique<ImplicitSubject>(ImplicitSubject::Kind::IDENTITY)
        );

        if (match.capturedLength("buzz_to")) {
            ping_event->as<RawPingEvent>()->pingee =
                parse_libpurple_subject(match.captured("buzz_to"), protocol, is_html);
        }

        return ping_event;
    } else if (match.capturedLength("buzz_to_fail")) {
        unique_ptr<RawEvent> ping_event = make_unique<RawPingEvent>(
            event_time,
            event_index,
            make_unique<ImplicitSubject>(ImplicitSubject::Kind::IDENTITY)
        );
        ping_event->as<RawPingEvent>()->pingee =
            parse_libpurple_subject(match.captured("buzz_to_fail"), protocol, is_html);
        ping_event->as<RawPingEvent>()->reasonFailed = PingFailReason::BLOCKED_OR_UNSUPPORTED;

        return ping_event;
    } else if (match.capturedLength("buzz_from")) {
        return make_unique<RawPingEvent>(
            event_time,
            event_index,
            parse_libpurple_subject(match.captured("buzz_from"), protocol, is_html)
        );
    } else if (match.capturedLength("rename_old")) {
        return make_unique<RawChangeScreenNameEvent>(
            event_time,
            event_index,
            parse_libpurple_subject(match.captured("rename_old"), protocol, is_html),
            parse_libpurple_subject(match.captured("rename_new"), protocol, is_html)
        );
    } else if (match.capturedLength("leave_11_who")) {
        return make_unique<RawLeaveConversationEvent>(
            event_time,
            event_index,
            parse_libpurple_subject(match.captured("leave_11_who"), protocol, is_html)
        );
    } else if (match.capturedLength("join_conf_who")) {
        return make_unique<RawJoinConferenceEvent>(
            event_time,
            event_index,
            parse_libpurple_subject(match.captured("join_conf_who"), protocol, is_html)
        );
    } else if (match.capturedLength("leave_conf_who")) {
        return make_unique<RawLeaveConferenceEvent>(
            event_time,
            event_index,
            parse_libpurple_subject(match.captured("leave_conf_who"), protocol, is_html)
        );
    } else if (match.capturedLength("sign_on_who")) {
        return make_unique<RawConnectedEvent>(
            event_time,
            event_index,
            parse_libpurple_subject(match.captured("sign_on_who"), protocol, is_html)
        );
    } else if (match.capturedLength("sign_off_who")) {
        return make_unique<RawDisconnectedEvent>(
            event_time,
            event_index,
            parse_libpurple_subject(match.captured("sign_off_who"), protocol, is_html)
        );
    } else if (match.capturedLength("away_who")) {
        return make_unique<RawStatusChangeEvent>(
            event_time,
            event_index,
            parse_libpurple_subject(match.captured("away_who"), protocol, is_html),
            IMStatus::AWAY
        );
    } else if (match.capturedLength("idle_who")) {
        return make_unique<RawStatusChangeEvent>(
            event_time,
            event_index,
            parse_libpurple_subject(match.captured("idle_who"), protocol, is_html),
            IMStatus::IDLE
        );
    } else if (match.capturedLength("avail_who")) {
        return make_unique<RawStatusChangeEvent>(
            event_time,
            event_index,
            parse_libpurple_subject(match.captured("avail_who"), protocol, is_html),
            IMStatus::AVAILABLE
        );
    } else if (match.capturedLength("msg_too_large")) {
        unique_ptr<RawEvent> event = make_unique<RawMessageEvent>(
            event_time,
            event_index,
            make_unique<ImplicitSubject>(ImplicitSubject::Kind::IDENTITY),
            RawMessageContent()
        );
        event->as<RawMessageEvent>()->reasonFailed = SendMessageFailReason::MESSAGE_TOO_LARGE;

        return event;
    } else if (match.capturedLength("msg_not_sent")) {
        invariant(!is_html, "'Message could not be sent' events not supported in HTML mode");

        unique_ptr<RawEvent> event = make_unique<RawMessageEvent>(
            event_time,
            event_index,
            make_unique<ImplicitSubject>(ImplicitSubject::Kind::IDENTITY),
            RawMessageContent::fromPlainText(match.captured("unsent_msg"))
        );

        if (match.capturedLength("fail_user_offline")) {
            event->as<RawMessageEvent>()->reasonFailed = SendMessageFailReason::RECIPIENT_OFFLINE;
        } else if (match.capturedLength("fail_conn_err")) {
            event->as<RawMessageEvent>()->reasonFailed = SendMessageFailReason::CONNECTION_ERROR;
        } else {
            never_reached();
        }

        return event;
    } else if (match.capturedLength("unsup_webcam_from")) {
        unique_ptr<RawEvent> cam_event = make_unique<RawOfferWebcamEvent>(
            event_time,
            event_index,
            parse_libpurple_subject(match.captured("unsup_webcam_from"), protocol, is_html)
        );
        cam_event->as<RawOfferWebcamEvent>()->reasonFailed = OfferWebcamFailReason::UNSUPPORTED;

        return cam_event;
    } else if (match.capturedLength("log_started")) {
        return make_unique<RawLoggingStartedEvent>(event_time, event_index);
    } else if (match.capturedLength("log_stopped")) {
        return make_unique<RawLoggingStoppedEvent>(event_time, event_index);
    }

    invariant_violation("Unsupported libpurple system message: %s", QP(content));
}

CEDE(ApparentSubject) parse_libpurple_subject(IMM(QString) subject, IMProtocol protocol, bool is_html) {
    if (!is_html) {
        return make_unique<SubjectGivenAsScreenName>(strip_subject_suffix(subject));
    }

    QREGEX_MATCH_CI(match, "^(.*) \\[<em>(.*)</em>\\]$", subject);
    if (match.hasMatch()) {
        QString screen_name = strip_subject_suffix(decode_html_entities(match.captured(1)));
        QString account_name = strip_subject_suffix(decode_html_entities(match.captured(2)));

        return make_unique<FullySpecifiedSubject>(
            parse_account_generic(protocol, account_name),
            screen_name
        );
    }

    return make_unique<SubjectGivenAsScreenName>(strip_subject_suffix(decode_html_entities(subject)));
}

static QString strip_subject_suffix(IMM(QString) subject) {
    QREGEX_MATCH_CI(match, "(.*@[^/]*)/.*", subject);
    return match.hasMatch() ? match.captured(1) : subject;
}

static RawTransferredFile parse_file(IMM(QString) filename_html, bool is_html) {
    QString decoded_filename;

    if (is_html) {
        QREGEX_MATCH_CI(link_match, "^<a href=\"(.*)\">.*</a>$", filename_html);
        decoded_filename = decode_html_entities(link_match.hasMatch() ? link_match.captured(1) : filename_html);
    } else {
        decoded_filename = filename_html;
    }

    return RawTransferredFile(decoded_filename);
}

}}
