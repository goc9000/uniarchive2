/**
 * extraction/skype/extract_skype_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "extraction/skype/extract_skype_conversations.h"
#include "extraction/skype/internal/RawSkypeConvo.h"
#include "extraction/skype/internal/RawSkypeChat.h"
#include "extraction/skype/internal/RawSkypeCall.h"
#include "extraction/skype/internal/RawSkypeIdentity.h"
#include "intermediate_format/content/formatting/LinkTag.h"
#include "intermediate_format/content/symbols/SkypeEmoticon.h"
#include "intermediate_format/content/symbols/FlagIcon.h"
#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/content/SkypeQuote.h"
#include "intermediate_format/content/TextSection.h"
#include "intermediate_format/errors/CurrentCallFailReason.h"
#include "intermediate_format/errors/StartCallFailReason.h"
#include "intermediate_format/events/calls/RawEndCallEvent.h"
#include "intermediate_format/events/calls/RawStartCallEvent.h"
#include "intermediate_format/events/conference/RawAddToConferenceEvent.h"
#include "intermediate_format/events/conference/RawChangeConferencePictureEvent.h"
#include "intermediate_format/events/conference/RawChangeTopicEvent.h"
#include "intermediate_format/events/conference/RawJoinConferenceEvent.h"
#include "intermediate_format/events/conference/RawLeaveConferenceEvent.h"
#include "intermediate_format/events/conference/RawRejoinConferenceEvent.h"
#include "intermediate_format/events/conference/RawRemoveFromConferenceEvent.h"
#include "intermediate_format/events/conference/RawSetSkypeChatRoleEvent.h"
#include "intermediate_format/events/conversation/RawJoinConversationEvent.h"
#include "intermediate_format/events/conversation/RawStartConversationEvent.h"
#include "intermediate_format/events/file_transfer/RawTransferFilesEvent.h"
#include "intermediate_format/events/friending/RawContactDeleteEvent.h"
#include "intermediate_format/events/friending/RawContactRequestEvent.h"
#include "intermediate_format/events/friending/RawContactRequestAcceptEvent.h"
#include "intermediate_format/events/RawEditedPreviousMessageEvent.h"
#include "intermediate_format/events/RawMessageEvent.h"
#include "intermediate_format/events/RawSendContactsEvent.h"
#include "intermediate_format/events/RawUninterpretedEvent.h"
#include "intermediate_format/provenance/ArchiveFileProvenance.h"
#include "intermediate_format/provenance/SkypeConversationProvenance.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/subjects/AccountSubject.h"
#include "intermediate_format/subjects/FullySpecifiedSubject.h"
#include "intermediate_format/RawConversation.h"
#include "protocols/skype/skype_account_name.h"
#include "protocols/ArchiveFormat.h"
#include "protocols/IMProtocol.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/qt/debug_extras.h"
#include "utils/sqlite/SQLiteDB.h"
#include "utils/xml/qdom_utils.h"

#include <set>
#include <map>

#include <QtDebug>
#include <QFileInfo>
#include <QUrl>

namespace uniarchive2 { namespace extraction { namespace skype {

using namespace std;
using namespace uniarchive2::extraction::skype::internal;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::errors;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::provenance;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;
using namespace uniarchive2::protocols::skype;
using namespace uniarchive2::utils::sqlite;
using namespace uniarchive2::utils::xml;

struct CallEventData {
    uint64_t eventID;
    uint64_t timestamp;
    uint64_t convoID;
    QString authorAccount;
    QString authorScreenName;
    QString bodyXML;
    QString identities;
    QString dialogPartner;
    QString failReason;
    optional<uint64_t> duration;
    optional<QString> callGUID;

    bool matches(uint64_t convo_id, IMM(QString) dialog_partner, IMM(optional<QString>) call_guid) {
        return (convoID == convo_id) && (dialogPartner == dialog_partner) && (callGUID == call_guid);
    }
};

static map<QString, RawSkypeIdentity> query_raw_skype_identities(SQLiteDB &db);
static map<uint64_t, RawSkypeConvo> query_raw_skype_convos(SQLiteDB &db);
static map<QString, RawSkypeChat> query_raw_skype_chats(SQLiteDB &db);
static map<uint64_t, RawSkypeCall> query_raw_skype_calls(SQLiteDB &db);

static map<QString, RawConversation> convert_conversations(
    SQLiteDB& db,
    const map<QString, RawSkypeIdentity>& raw_identities,
    const map<uint64_t, RawSkypeConvo>& raw_convos,
    const map<QString, RawSkypeChat>& raw_chats,
    const map<uint64_t, RawSkypeCall>& raw_calls,
    CPTR(Provenance) base_provenance
);
static RawConversation convert_one_on_one_conversation(
    IMM(RawSkypeConvo) skype_convo,
    CPTR(RawSkypeChat) skype_chat,
    const map<QString, RawSkypeIdentity>& raw_identities,
    TAKE(SkypeConversationProvenance) provenance
);
static RawConversation convert_group_chat(
    IMM(RawSkypeConvo) skype_convo,
    CPTR(RawSkypeChat) skype_chat,
    const map<QString, RawSkypeIdentity>& raw_identities,
    TAKE(SkypeConversationProvenance) provenance
);
static CEDE(ApparentSubject) extract_identity_from_participants(
    set<QString>& mut_participants,
    const map<QString, RawSkypeIdentity>& raw_identities
);
static optional<uint64_t> extract_call_id(QString chat_string_id);
static CPTR(RawSkypeCall) lookup_call(
    const map<uint64_t, RawSkypeCall>& raw_calls,
    uint64_t skype_convo_id,
    QString chat_string_id
);
static RawConversation convert_call(
    IMM(RawSkypeConvo) skype_convo,
    CPTR(RawSkypeCall) skype_call,
    const map<QString, RawSkypeIdentity>& raw_identities,
    TAKE(SkypeConversationProvenance) provenance
);
static CEDE(ApparentSubject) make_call_subject(IMM(QString) account_name, CPTR(RawSkypeCall) skype_call);

static void convert_events(
    SQLiteDB& db,
    map<QString, RawConversation>& mut_indexed_conversations,
    map<uint64_t, unique_ptr<RawEvent>>&& prescanned_call_events
);
static CEDE(RawEvent) convert_event(
    IMM(ApparentTime) event_time,
    uint32_t event_index,
    int type,
    int chatmsg_type,
    TAKE(ApparentSubject) subject,
    IMM(QString) body_xml,
    vector<unique_ptr<ApparentSubject>>&& identities,
    IMM(optional<QString>) edited_by,
    IMM(optional<uint64_t>) edited_timestamp,
    IMM(optional<int>) new_role,
    IMM(RawConversation) home_conversation
);
static CEDE(RawMessageEvent) convert_message_event(
    IMM(ApparentTime) event_time,
    uint32_t event_index,
    TAKE(ApparentSubject) subject,
    IMM(QString) body_xml,
    bool is_action_message,
    IMM(optional<QString>) edited_by,
    IMM(optional<uint64_t>) edited_timestamp
);

static vector<CEDE(ApparentSubject)> deserialize_identities(IMM(optional<QString>) serialized_identities);

static RawMessageContent parse_message_content(IMM(QString) content_xml);
static void parse_message_content_node(RawMessageContent& mut_content, IMM(QDomNode) node);
static void parse_message_content_element(RawMessageContent& mut_content, IMM(QDomElement) element);
static CEDE(SkypeQuote) parse_quote_element(IMM(QDomElement) element);

static CEDE(RawSendContactsEvent) convert_send_contacts_event(
    IMM(ApparentTime) event_time,
    uint32_t event_index,
    TAKE(ApparentSubject) subject,
    IMM(QString) body_xml
);
static CEDE(RawEvent) convert_complex_join_event(
    IMM(ApparentTime) event_time,
    uint32_t event_index,
    TAKE(ApparentSubject) subject,
    TAKE_VEC(ApparentSubject) identities,
    IMM(RawConversation) home_conversation
);
static CEDE(RawEvent) convert_file_transfer_event(
    IMM(ApparentTime) event_time,
    uint32_t event_index,
    TAKE(ApparentSubject) subject,
    IMM(QString) body_xml
);

static map<uint64_t, unique_ptr<RawEvent>> prescan_call_events(
    SQLiteDB& db,
    const map<uint64_t, RawSkypeCall>& raw_calls
);
static void create_call_events(
    map<uint64_t, unique_ptr<RawEvent>>& mut_prescanned_call_events,
    IMM(CallEventData) start_event_data,
    IMM(CallEventData) end_event_data,
    const map<uint64_t, RawSkypeCall>& raw_calls
);
static CPTR(RawSkypeCall) find_corresponding_call(
    const map<uint64_t, RawSkypeCall>& raw_calls,
    uint64_t start_timestamp,
    uint64_t end_timestamp,
    uint64_t convo_id
);
static vector<CEDE(ApparentSubject)> get_call_event_peers(
   IMM(CallEventData) start_event_data,
   IMM(CallEventData) end_event_data,
   CPTR(RawSkypeCall) skype_call
);
static StartCallFailReason parse_start_call_fail_reason(IMM(QString) raw_reason);
static CurrentCallFailReason parse_current_call_fail_reason(IMM(QString) raw_reason);


vector<RawConversation> extract_skype_conversations(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", QP(filename));

    unique_ptr<Provenance> base_provenance = ArchiveFileProvenance::fromQFileInfo(ArchiveFormat::SKYPE, file_info);

    SQLiteDB db = SQLiteDB::openReadOnly(filename);
    map<QString, RawSkypeIdentity> raw_identities = query_raw_skype_identities(db);
    map<uint64_t, RawSkypeConvo> raw_convos = query_raw_skype_convos(db);
    map<QString, RawSkypeChat> raw_chats = query_raw_skype_chats(db);
    map<uint64_t, RawSkypeCall> raw_calls = query_raw_skype_calls(db);

    map<uint64_t, unique_ptr<RawEvent>> prescanned_call_events = prescan_call_events(db, raw_calls);

    map<QString, RawConversation> indexed_conversations =
        convert_conversations(db, raw_identities, raw_convos, raw_chats, raw_calls, base_provenance.get());

    convert_events(db, indexed_conversations, move(prescanned_call_events));

    vector<RawConversation> conversations;
    for (auto& kv : indexed_conversations) {
        conversations.push_back(move(kv.second));
    }

    return conversations;
}

static map<QString, RawSkypeIdentity> query_raw_skype_identities(SQLiteDB& db) {
    map<QString, RawSkypeIdentity> identities = db.stmt(
        "SELECT skypename, fullname FROM Accounts"
    ).mapRowsToMap(
        [](QString account_name, QString screen_name) -> pair<QString, RawSkypeIdentity> {
            return make_pair(account_name, RawSkypeIdentity(account_name, screen_name));
        }
    );

    return identities;
};

static map<uint64_t, RawSkypeConvo> query_raw_skype_convos(SQLiteDB& db) {
    map<uint64_t, RawSkypeConvo> convos = db.stmt(
        "SELECT id, type, identity, displayname, given_displayname, spawned_from_convo_id, creator, "\
        "       creation_timestamp, meta_topic "\
        "FROM Conversations"
    ).mapRowsToMap(
        [](
            uint64_t id,
            int type,
            QString identity,
            QString name,
            optional<QString> given_name,
            optional<uint64_t> spawned_from,
            optional<QString> creator,
            uint64_t time_created,
            optional<QString> last_topic
        ) -> pair<uint64_t, RawSkypeConvo> {
            return make_pair(
                id,
                RawSkypeConvo(
                    id, (RawSkypeConvo::Type)type, identity, name, given_name, spawned_from, creator,
                    time_created, last_topic
                )
            );
        });

    db.stmt("SELECT convo_id, identity FROM Participants").forEachRow(
        [&convos](uint64_t convo_id, QString identity) {
            invariant(convos.count(convo_id), "Conversation ID=%llu not found for participant", convo_id);
            invariant(
                !convos.at(convo_id).participants.count(identity),
                "Dupe participant '%s' in conversation ID=%llu", QP(identity), convo_id
            );

            convos.at(convo_id).participants.insert(identity);
        });

    return convos;
}

static map<QString, RawSkypeChat> query_raw_skype_chats(SQLiteDB& db) {
    map<QString, RawSkypeChat> chats = db.stmt(
        "SELECT id, name, type, friendlyname, timestamp, topic, topic_xml, conv_dbid FROM Chats"
    ).mapRowsToMap(
        [](
            uint64_t id,
            QString string_id,
            int type,
            QString friendly_name,
            uint64_t timestamp,
            optional<QString> last_topic,
            optional<QString> last_topic_xml,
            uint64_t conv_dbid
        ) -> pair<QString, RawSkypeChat> {
            return make_pair(
                string_id,
                RawSkypeChat(
                    id, string_id, (RawSkypeChat::Type)type, friendly_name, timestamp, last_topic, last_topic_xml,
                    conv_dbid
                )
            );
        });

    db.stmt("SELECT chatname, identity FROM ChatMembers").forEachRow(
        [&chats](QString chat_string_id, QString identity) {
            invariant(chats.count(chat_string_id), "Chat ID='%s' not found for participant", QP(chat_string_id));
            invariant(
                !chats.at(chat_string_id).participants.count(identity),
                "Dupe participant '%s' in chat ID='%s'", QP(identity), QP(chat_string_id)
            );

            chats.at(chat_string_id).participants.insert(identity);
        }
    );

    return chats;
}

static map<uint64_t, RawSkypeCall> query_raw_skype_calls(SQLiteDB& db) {
    map<uint64_t, RawSkypeCall> calls = db.stmt(
        "SELECT id, host_identity, is_incoming, begin_timestamp, duration, topic, name, conv_dbid "\
        "FROM Calls"
    ).mapRowsToMap(
        [](
            uint64_t id,
            QString host_identity,
            bool is_incoming,
            uint64_t begin_timestamp,
            optional<uint> duration,
            QString topic,
            QString internal_name,
            uint64_t conv_dbid
        ) -> pair<uint64_t, RawSkypeCall> {
            return make_pair(
                id,
                RawSkypeCall(id, host_identity, is_incoming, begin_timestamp, duration, topic, internal_name, conv_dbid)
            );
        });

    db.stmt("SELECT call_db_id, identity, dispname FROM CallMembers").forEachRow(
        [&calls](uint64_t call_id, QString identity, QString dispname) {
            invariant(calls.count(call_id), "Call ID=%llu not found for participant", call_id);
            invariant(
                !calls.at(call_id).participants.count(identity),
                "Dupe participant '%s' in call ID=%llu", QP(identity), call_id
            );

            calls.at(call_id).participants.emplace(identity, dispname);
        });

    return calls;
}

static map<QString, RawConversation> convert_conversations(
    SQLiteDB& db,
    const map<QString, RawSkypeIdentity>& raw_identities,
    const map<uint64_t, RawSkypeConvo>& raw_convos,
    const map<QString, RawSkypeChat>& raw_chats,
    const map<uint64_t, RawSkypeCall>& raw_calls,
    CPTR(Provenance) base_provenance
) {
    map<QString, RawConversation> conversations;

    db.stmt(
        "SELECT chatname, convo_id FROM Messages "\
        "GROUP BY chatname, convo_id"
    ).forEachRow(
        [&conversations, &raw_convos, &raw_chats, &raw_calls, &raw_identities, &base_provenance](
            QString chat_string_id,
            uint64_t convo_id
        ) -> void {
            QString key = chat_string_id.isEmpty() ? QString::number(convo_id) : chat_string_id;
            invariant(!conversations.count(key), "Multiple conversations map to key: %s", QP(key));

            IMM(RawSkypeConvo) convo = raw_convos.at(convo_id);
            CPTR(RawSkypeChat) chat = raw_chats.count(chat_string_id) ? &raw_chats.at(chat_string_id) : nullptr;
            CPTR(RawSkypeCall) call = nullptr;

            unique_ptr<SkypeConversationProvenance> provenance =
                make_unique<SkypeConversationProvenance>(base_provenance->clone(), convo_id, chat_string_id);

            switch (convo.type) {
                case RawSkypeConvo::Type::ONE_ON_ONE:
                    if (chat && chat->type != RawSkypeChat::Type::ONE_ON_ONE) {
                        // Some group chats start with a (spurious) association with a one-on-one convo. Ignore this.
                        return;
                    }
                    conversations.emplace(
                        key,
                        convert_one_on_one_conversation(convo, chat, raw_identities, move(provenance))
                    );
                    return;
                case RawSkypeConvo::Type::GROUP_CHAT:
                    conversations.emplace(key, convert_group_chat(convo, chat, raw_identities, move(provenance)));
                    return;
                case RawSkypeConvo::Type::CALL:
                    call = lookup_call(raw_calls, convo_id, chat_string_id);
                    conversations.emplace(key, convert_call(convo, call, raw_identities, move(provenance)));
                    return;
            }
        });

    return conversations;
}

static RawConversation convert_one_on_one_conversation(
    IMM(RawSkypeConvo) skype_convo,
    CPTR(RawSkypeChat) skype_chat,
    const map<QString, RawSkypeIdentity>& raw_identities,
    TAKE(SkypeConversationProvenance) provenance
) {
    invariant(skype_convo.type == RawSkypeConvo::Type::ONE_ON_ONE, "Expected 1:1 Skype convo");
    invariant(!skype_chat || (skype_chat->type == RawSkypeChat::Type::ONE_ON_ONE), "Expected 1:1 Skype chat");
    invariant(!skype_chat || (skype_chat->convDBID == skype_convo.id), "Chat ConvDBID mismatch");

    invariant(skype_convo.participants.size() <= 2, "Expected at most 2 participants for a 1:1 conversation");

    if (skype_chat) {
        invariant(
            includes(
                skype_convo.participants.cbegin(),
                skype_convo.participants.cend(),
                skype_chat->participants.cbegin(),
                skype_chat->participants.cend()
            ),
            "Expected Skype chat participants to be included in convo participants"
        );
    }

    set<QString> participants = skype_convo.participants;
    invariant(participants.count(skype_convo.identity), "convo.identity not found in participants");
    participants.erase(skype_convo.identity);

    QString identity = participants.empty() ? skype_convo.identity : *participants.cbegin();
    invariant(raw_identities.count(identity), "Expected '%s' to be an identity", QP(identity));

    RawConversation conversation(IMProtocol::SKYPE);
    conversation.isConference = false;
    conversation.identity = make_unique<FullySpecifiedSubject>(
        parse_skype_account(identity),
        raw_identities.at(identity).screenName
    );
    conversation.declaredPeers.emplace_back(
        make_unique<AccountSubject>(parse_skype_account(skype_convo.identity))
    );

    if (skype_chat) {
        conversation.declaredStartDate = ApparentTime::fromUnixTimestamp(skype_chat->timestamp);
    }

    if (skype_chat) {
        provenance->chatDBID = skype_chat->id;
    }
    conversation.provenance = move(provenance);

    return conversation;
}

static RawConversation convert_group_chat(
    IMM(RawSkypeConvo) skype_convo,
    CPTR(RawSkypeChat) skype_chat,
    const map<QString, RawSkypeIdentity>& raw_identities,
    TAKE(SkypeConversationProvenance) provenance
) {
    invariant(skype_convo.type == RawSkypeConvo::Type::GROUP_CHAT, "Expected group Skype convo");
    invariant(!skype_chat || (skype_chat->type == RawSkypeChat::Type::GROUP_CHAT), "Expected group Skype chat");
    invariant(!skype_chat || (skype_chat->convDBID == skype_convo.id), "Chat ConvDBID mismatch");

    if (skype_chat) {
        invariant(
            includes(
                skype_convo.participants.cbegin(),
                skype_convo.participants.cend(),
                skype_chat->participants.cbegin(),
                skype_chat->participants.cend()
            ),
            "Expected Skype chat participants to be included in convo participants"
        );
    }

    set<QString> participants = skype_convo.participants;

    RawConversation conversation(IMProtocol::SKYPE);
    conversation.isConference = true;
    conversation.identity = extract_identity_from_participants(participants, raw_identities);

    if (skype_convo.creator) {
        QString initiator = *skype_convo.creator;
        conversation.declaredInitiator = make_unique<AccountSubject>(parse_skype_account(initiator));

        if (participants.count(initiator)) {
            participants.erase(initiator);
            conversation.declaredPeers.emplace_back(make_unique<AccountSubject>(parse_skype_account(initiator)));
        }
    }
    for (IMM(QString) participant : participants) {
        conversation.declaredPeers.emplace_back(make_unique<AccountSubject>(parse_skype_account(participant)));
    }

    // The >10^9 check is necessary because sometimes the chat date is spurious
    conversation.declaredStartDate = ApparentTime::fromUnixTimestamp(
        (skype_chat && (skype_chat->timestamp > 1000000000)) ? skype_chat->timestamp : skype_convo.timeCreated
    );

    conversation.conferenceTitle = skype_chat ? skype_chat->friendlyName : skype_convo.displayName;
    if (skype_convo.givenDisplayName) {
        conversation.conferenceTitle = *skype_convo.givenDisplayName;
    }

    if (skype_chat) {
        provenance->chatDBID = skype_chat->id;
    }
    conversation.provenance = move(provenance);

    return conversation;
}

static CEDE(ApparentSubject) extract_identity_from_participants(
    set<QString>& mut_participants,
    const map<QString, RawSkypeIdentity>& raw_identities
) {
    QString identity_account;

    for (IMM(QString) participant : mut_participants) {
        if (raw_identities.count(participant)) {
            invariant(
                identity_account.isEmpty(),
                "More than one identity in participant list (%s and %s)", QP(identity_account), QP(participant)
            );

            identity_account = participant;
        }
    }

    invariant(!identity_account.isEmpty(), "No identity account found in participant list");

    mut_participants.erase(identity_account);

    return make_unique<FullySpecifiedSubject>(
        parse_skype_account(identity_account),
        raw_identities.at(identity_account).screenName
    );
}

static optional<uint64_t> extract_call_id(QString chat_string_id) {
    optional<uint64_t> result;

    QREGEX_MATCH_CI(match, "#[^/]*/\\$\\*\\d+;(\\d+)", chat_string_id);
    if (match.hasMatch()) {
        result = match.captured(1).toInt();
    }

    return result;
}

static CPTR(RawSkypeCall) lookup_call(
    const map<uint64_t, RawSkypeCall>& raw_calls,
    uint64_t skype_convo_id,
    QString chat_string_id
) {
    optional<uint64_t> call_id = extract_call_id(chat_string_id);
    if (call_id) {
        invariant(raw_calls.count(*call_id), "Call ID=%lld not found", *call_id);
        return &raw_calls.at(*call_id);
    }

    CPTR(RawSkypeCall) call = nullptr;

    for (IMM(auto) kv : raw_calls) {
        if (kv.second.convDBID == skype_convo_id) {
            invariant(!call, "Multiple calls found for conversation ID=%lld", skype_convo_id);
            call = &kv.second;
        }
    }

    return call;
}

static RawConversation convert_call(
    IMM(RawSkypeConvo) skype_convo,
    CPTR(RawSkypeCall) skype_call,
    const map<QString, RawSkypeIdentity>& raw_identities,
    TAKE(SkypeConversationProvenance) provenance
) {
    invariant(skype_convo.type == RawSkypeConvo::Type::CALL, "Expected call Skype convo");
    invariant(!skype_call || (skype_call->convDBID == skype_convo.id), "Call ConvDBID mismatch");

    if (skype_call) {
        for (IMM(auto) kv : skype_call->participants) {
            invariant(
                skype_convo.participants.count(kv.first),
                "Expected Skype call participants to be included in convo participants"
            );
        }
    }

    set<QString> participants = skype_convo.participants;

    RawConversation conversation(IMProtocol::SKYPE);
    conversation.identity = extract_identity_from_participants(participants, raw_identities);

    QString initiator = skype_call ? skype_call->hostIdentity : (skype_convo.creator ? *skype_convo.creator : "");
    if (!initiator.isEmpty()) {
        conversation.declaredInitiator = make_call_subject(initiator, skype_call);
        if (participants.count(initiator)) {
            conversation.declaredPeers.emplace_back(make_call_subject(initiator, skype_call));
            participants.erase(initiator);
        }
    }
    for (IMM(QString) participant : participants) {
        conversation.declaredPeers.emplace_back(make_call_subject(participant, skype_call));
    }

    conversation.declaredStartDate = ApparentTime::fromUnixTimestamp(
        skype_call ? skype_call->beginTimestamp : skype_convo.timeCreated
    );

    if (skype_call && !skype_call->topic.isEmpty()) {
        conversation.conferenceTitle = skype_call->topic;
    }

    if (skype_call) {
        provenance->callDBID = skype_call->id;
    }
    conversation.provenance = move(provenance);

    return conversation;
}

static CEDE(ApparentSubject) make_call_subject(IMM(QString) account_name, CPTR(RawSkypeCall) skype_call) {
    if (skype_call && skype_call->participants.count(account_name)) {
        return make_unique<FullySpecifiedSubject>(
            parse_skype_account(account_name),
            skype_call->participants.at(account_name)
        );
    }

    return make_unique<AccountSubject>(parse_skype_account(account_name));
}

static void convert_events(
    SQLiteDB& db,
    map<QString, RawConversation>& mut_indexed_conversations,
    map<uint64_t, unique_ptr<RawEvent>>&& prescanned_call_events
) {
    db.stmt(
        "SELECT chatname, convo_id, id, timestamp, type, chatmsg_type, author, from_dispname, body_xml, "\
        "       identities, guid, edited_by, edited_timestamp, newrole "\
        "FROM Messages "\
        "ORDER BY timestamp, id"
    ).forEachRow(
        [&mut_indexed_conversations, &prescanned_call_events](
            QString chat_string_id,
            uint64_t convo_id,
            uint64_t event_id,
            uint64_t timestamp,
            int type,
            int chatmsg_type,
            QString author,
            QString from_dispname,
            QString body_xml,
            optional<QString> serialized_identities,
            optional<QByteArray> skype_guid,
            optional<QString> edited_by,
            optional<uint64_t> edited_timestamp,
            optional<int> new_role
        ) -> void {
            QString key = chat_string_id.isEmpty() ? QString::number(convo_id) : chat_string_id;
            RawConversation& mut_conversation = mut_indexed_conversations.at(key);

            ApparentTime event_time = ApparentTime::fromUnixTimestamp(timestamp);
            uint32_t event_index = (uint32_t)mut_conversation.events.size();

            auto subject = make_unique<FullySpecifiedSubject>(parse_skype_account(author), from_dispname);
            auto identities = deserialize_identities(serialized_identities);

            unique_ptr<RawEvent> event;

            if (prescanned_call_events.count(event_id)) {
                event = move(prescanned_call_events.at(event_id));
                if (event) {
                    event->indexInConversation = event_index;
                }
                prescanned_call_events.erase(event_id);
            } else {
                event = convert_event(
                    event_time,
                    event_index,
                    type,
                    chatmsg_type,
                    move(subject),
                    body_xml,
                    move(identities),
                    edited_by,
                    edited_timestamp,
                    new_role,
                    mut_conversation
                );
            }

            if (!event) {
                return;
            }

            if (skype_guid) {
                event->skypeGUID = *skype_guid;
            }

            mut_conversation.events.push_back(move(event));
        });
}

static CEDE(RawEvent) convert_event(
    IMM(ApparentTime) event_time,
    uint32_t event_index,
    int type,
    int chatmsg_type,
    TAKE(ApparentSubject) subject,
    IMM(QString) body_xml,
    TAKE_VEC(ApparentSubject) identities,
    IMM(optional<QString>) edited_by,
    IMM(optional<uint64_t>) edited_timestamp,
    IMM(optional<int>) new_role,
    IMM(RawConversation) home_conversation
) {
#define COMBINED_TYPE(t, ct) ((t << 8) + ct)
    invariant((type >= 0) && (type <= 255), "Expected message type to be a byte, found %d", type);
    invariant((chatmsg_type >= 0) && (chatmsg_type <= 255), "Expected byte for chatmsg_type, found %d", chatmsg_type);

    unique_ptr<RawEvent> event;

    switch (COMBINED_TYPE(type, chatmsg_type)) {
        // Basic

        case COMBINED_TYPE(61, 3):
        case COMBINED_TYPE(61, 0):
        case COMBINED_TYPE(60, 7):
            return convert_message_event(
                event_time,
                event_index,
                move(subject),
                body_xml,
                (type == 60), // is_action_message
                edited_by,
                edited_timestamp
            );
        case COMBINED_TYPE(68, 0):
        case COMBINED_TYPE(68, 7):
        case COMBINED_TYPE(68, 18):
            return convert_file_transfer_event(event_time, event_index, move(subject), body_xml);
        case COMBINED_TYPE(0, 103):
            return make_unique<RawEditedPreviousMessageEvent>(event_time, event_index, move(subject));

        // Conference

        case COMBINED_TYPE(10, 1):
        case COMBINED_TYPE(10, 2):
            invariant(identities.size() > 0, "Expected at least one subject for conference add");
            event = make_unique<RawAddToConferenceEvent>(
                event_time,
                event_index,
                move(subject),
                move(identities)
            );
            event->as<RawAddToConferenceEvent>()->asModerator = (chatmsg_type == 2);

            return event;
        case COMBINED_TYPE(12, 11):
            invariant(identities.size() == 1, "Expected exactly 1 subject for kick");
            return make_unique<RawRemoveFromConferenceEvent>(
                event_time,
                event_index,
                move(subject),
                move(identities.front())
            );
        case COMBINED_TYPE(13, 4):
            return make_unique<RawLeaveConferenceEvent>(event_time, event_index, move(subject));
        case COMBINED_TYPE(21, 10):
            invariant(identities.size() == 1, "Expected exactly 1 subject for set role");
            invariant(new_role, "new_role needs to be set for 'set role' event");
            invariant((*new_role >= 1) && (*new_role <= 5), "invalid new_role: %d", *new_role);

            return make_unique<RawSetSkypeChatRoleEvent>(
                event_time,
                event_index,
                move(subject),
                move(identities.front()),
                (SkypeChatRole)(*new_role)
            );
        case COMBINED_TYPE(2, 5):
            return make_unique<RawChangeTopicEvent>(
                event_time,
                event_index,
                move(subject),
                parse_message_content(body_xml)
            );
        case COMBINED_TYPE(2,15):
            return make_unique<RawChangeConferencePictureEvent>(event_time, event_index, move(subject));

        // Joining

        case COMBINED_TYPE(0, 100):
            if (*home_conversation.isConference) {
                return make_unique<RawJoinConferenceEvent>(event_time, event_index, move(subject));
            } else {
                return make_unique<RawJoinConversationEvent>(event_time, event_index, move(subject));
            }
        case COMBINED_TYPE(100, 2):
            invariant(identities.size() == 1, "Expected exactly 1 peer for start conversation");
            return make_unique<RawStartConversationEvent>(
                event_time,
                event_index,
                move(subject),
                move(identities.front())
            );
        case COMBINED_TYPE(100, 6):
            return convert_complex_join_event(
                event_time,
                event_index,
                move(subject),
                move(identities),
                home_conversation
            );

        // Friending

        case COMBINED_TYPE(50, 0):
            invariant(identities.size() == 1, "Expected exactly 1 receiver for friend request");
            return make_unique<RawContactRequestEvent>(
                event_time,
                event_index,
                move(subject),
                move(identities.front()),
                parse_message_content(body_xml)
            );
        case COMBINED_TYPE(51, 18):
            invariant(identities.size() == 1, "Expected exactly 1 subject for friend accept");
            return make_unique<RawContactRequestAcceptEvent>(
                event_time,
                event_index,
                move(subject),
                move(identities.front())
            );
        case COMBINED_TYPE(110, 0):
            invariant(identities.size() == 1, "Expected exactly 1 identity for unfriend event");
            return make_unique<RawContactDeleteEvent>(
                event_time,
                event_index,
                move(identities.front()),
                move(subject)
            );

       // Misc

       case COMBINED_TYPE(63, 8):
            return convert_send_contacts_event(event_time, event_index, move(subject), body_xml);
    }



// ADD new MESSAGES

// FIX CALLS (delta timestamp != param)




    // Default
    QString tmp;
    QDebug dbg(&tmp);

    return make_unique<RawUninterpretedEvent>(event_time, event_index, tmp.toUtf8() + "?");



    // TODO: fix this


    invariant_violation("Unsupported Skype event type (type=%d, chatmsg_type=%d)", type, chatmsg_type);
}

static vector<CEDE(ApparentSubject)> deserialize_identities(IMM(optional<QString>) serialized_identities) {
    vector<unique_ptr<ApparentSubject>> result;

    if (serialized_identities && !serialized_identities->isEmpty()) {
        for (IMM(QString) item : serialized_identities->split(" ")) {
            result.push_back(make_unique<AccountSubject>(parse_skype_account(item)));
        }
    }

    return result;
}

static CEDE(RawMessageEvent) convert_message_event(
    IMM(ApparentTime) event_time,
    uint32_t event_index,
    TAKE(ApparentSubject) subject,
    IMM(QString) body_xml,
    bool is_action_message,
    IMM(optional<QString>) edited_by,
    IMM(optional<uint64_t>) edited_timestamp
) {
    unique_ptr<RawMessageEvent> message = make_unique<RawMessageEvent>(
        event_time,
        event_index,
        move(subject),
        parse_message_content(body_xml)
    );

    message->isAction = is_action_message;

    if (edited_by) {
        invariant(edited_timestamp, "Edit timestamp not specified!");

        message->isEdited = true;
        message->editedBy = make_unique<AccountSubject>(parse_skype_account(*edited_by));
        message->timeEdited = ApparentTime::fromUnixTimestamp(*edited_timestamp);
    }

    return message;
}

static RawMessageContent parse_message_content(IMM(QString) content_xml) {
    RawMessageContent content;

    QDomDocument xml = xml_from_fragment_string(content_xml, "event", true);

    parse_message_content_node(content, get_dom_root(xml, "event"));

    return content;
}

static void parse_message_content_node(RawMessageContent& mut_content, IMM(QDomNode) node) {
    if (node.isElement()) {
        parse_message_content_element(mut_content, node.toElement());
    } else if (node.isText()) {
        IMM(QString) text = node.toText().data();
        if (!text.isEmpty()) {
            mut_content.addItem(make_unique<TextSection>(text));
        }
    } else {
        invariant_violation("Expected node to be either element or text");
    }
}

static void parse_message_content_element(RawMessageContent& mut_content, IMM(QDomElement) element) {
    IMM(QString) tag_name = element.tagName();

    if (tag_name == "event") {
        for (QDomNode child = element.firstChild(); !child.isNull(); child = child.nextSibling()) {
            parse_message_content_node(mut_content, child);
        }
    } else if (tag_name == "ss") {
        mut_content.addItem(make_unique<SkypeEmoticon>(
            read_only_string_attr(element, "type"),
            read_text_only_content(element)
        ));
    } else if (tag_name == "flag") {
        mut_content.addItem(make_unique<FlagIcon>(
            read_only_string_attr(element, "country"),
            read_text_only_content(element)
        ));
    } else if (tag_name == "a") {
        mut_content.addItem(make_unique<LinkTag>(QUrl(read_only_string_attr(element, "href"))));

        for (QDomNode child = element.firstChild(); !child.isNull(); child = child.nextSibling()) {
            parse_message_content_node(mut_content, child);
        }

        mut_content.addItem(make_unique<LinkTag>(false));
    } else if (tag_name == "quote") {
        mut_content.addItem(parse_quote_element(element));
    } else if (tag_name == "e_m") {
        // Ignore this tag (a redundant edit marker)
    } else {
        invariant_violation("Unsupported message node: <%s>", QP(tag_name));
    }
}

static CEDE(SkypeQuote) parse_quote_element(IMM(QDomElement) element) {
    IMM(QDomNodeList) children = element.childNodes();

    invariant(children.count() >= 2, "Expected <quote> to have at least 2 subnodes");
    invariant(
        element.firstChild().isElement() &&
        (element.firstChild().toElement().tagName() == "legacyquote"),
        "Expected first element in <quote> to be <legacyquote>"
    );
    invariant(
        element.lastChild().isElement() &&
        (element.lastChild().toElement().tagName() == "legacyquote") &&
        (read_text_only_content(element.lastChild().toElement()).trimmed() == "<<<"),
        "Expected last element in <quote> to be <legacyquote> containing the text '<<<'"
    );

    RawMessageContent quote_content;
    for (int i = 1; i < children.count() - 1; i++) {
        parse_message_content_node(quote_content, children.at(i));
    }

    return make_unique<SkypeQuote>(
        read_string_attr(element, "conversation"),
        read_string_attr(element, "guid"),
        make_unique<FullySpecifiedSubject>(
            parse_skype_account(read_string_attr(element, "author")),
            read_string_attr(element, "authorname")
        ),
        ApparentTime::fromUnixTimestamp(read_int_attr(element, "timestamp")),
        read_text_only_content(element.firstChild().toElement()),
        move(quote_content)
    );
}

static CEDE(RawSendContactsEvent) convert_send_contacts_event(
    IMM(ApparentTime) event_time,
    uint32_t event_index,
    TAKE(ApparentSubject) subject,
    IMM(QString) body_xml
) {
    QDomDocument xml = xml_from_fragment_string(body_xml, "event");

    vector<unique_ptr<ApparentSubject>> contacts;
    for (
        QDomElement element = only_child_elem(get_dom_root(xml, "event"), "contacts").firstChildElement();
        !element.isNull();
        element = element.nextSiblingElement()
    ) {
        invariant(element.tagName() == "c", "Expected <contacts> to contain only <c> tags");
        invariant(read_string_attr(element, "t") == "s", "Can handle only <c t=\"s\" ...> contact tags");

        contacts.push_back(make_unique<FullySpecifiedSubject>(
            parse_skype_account(read_string_attr(element, "s")),
            read_string_attr(element, "f")
        ));
    }

    return make_unique<RawSendContactsEvent>(event_time, event_index, move(subject), move(contacts));
}

static CEDE(RawEvent) convert_complex_join_event(
    IMM(ApparentTime) event_time,
    uint32_t event_index,
    TAKE(ApparentSubject) subject,
    TAKE_VEC(ApparentSubject) identities,
    IMM(RawConversation) home_conversation
) {
    QString subject_account = subject->as<FullySpecifiedSubject>()->accountName.accountName;

    for (auto id = identities.begin(); id != identities.end(); id++) {
        if ((*id)->as<AccountSubject>()->account.accountName == subject_account) {
            identities.erase(id);
            break;
        }
    }

    if (!*home_conversation.isConference) {
        invariant(identities.size() == 1, "Expected exactly 1 peer identity for 1:1 convo rejoin event");
        invariant(home_conversation.declaredPeers.size() == 1, "Expected exactly 1 peer identity for conversation");
        invariant(
            identities.front()->as<AccountSubject>()->account.accountName ==
            home_conversation.declaredPeers.front()->as<AccountSubject>()->account.accountName,
            "Identity should be the same as the unique peer for 1:1 convo rejoin events"
        );

        // Ignore this event as it will always be redundant with a RawJoinConversationEvent occuring at most
        // several seconds later
        return unique_ptr<RawEvent>();
    }

    return make_unique<RawRejoinConferenceEvent>(event_time, event_index, move(subject), move(identities));
}

static CEDE(RawEvent) convert_file_transfer_event(
    IMM(ApparentTime) event_time,
    uint32_t event_index,
    TAKE(ApparentSubject) subject,
    IMM(QString) body_xml
) {
    QDomDocument xml = xml_from_fragment_string(body_xml, "root");
    QDomElement files_elem = only_child_elem(get_dom_root(xml, "root"), "files");

    vector<RawTransferredFile> files;

    for (
         QDomElement file_elem = files_elem.firstChildElement();
         !file_elem.isNull();
         file_elem = file_elem.nextSiblingElement()
    ) {
        invariant(file_elem.tagName() == "file", "Expected only <file> children in <files>");
        files.emplace_back(read_text_only_content(file_elem), (uint64_t)read_int_attr(file_elem, "size"));
    }

    return make_unique<RawTransferFilesEvent>(event_time, event_index, move(subject), files);
}

static map<uint64_t, unique_ptr<RawEvent>> prescan_call_events(
    SQLiteDB& db,
    const map<uint64_t, RawSkypeCall>& raw_calls
) {
    map<uint64_t, unique_ptr<RawEvent>> prescanned_call_events;
    vector<CallEventData> open_events;

    db.stmt(
        "SELECT id, type, timestamp, convo_id, author, from_dispname, body_xml, identities, dialog_partner, reason, "\
        "       param_value, call_guid "\
        "FROM Messages "\
        "WHERE type IN (30, 39) "\
        "ORDER BY timestamp, id"
    ).forEachRow(
        [&open_events, &raw_calls, &prescanned_call_events](
            uint64_t event_id,
            int type,
            uint64_t timestamp,
            uint64_t convo_id,
            QString author,
            QString from_dispname,
            QString body_xml,
            QString identities,
            QString dialog_partner,
            QString fail_reason,
            optional<uint64_t> duration,
            optional<QString> call_guid
        ) -> void {
            CallEventData event_data {
                .eventID = event_id,
                .timestamp = timestamp,
                .convoID = convo_id,
                .authorAccount = author,
                .authorScreenName = from_dispname,
                .bodyXML = body_xml,
                .identities = identities,
                .dialogPartner = dialog_partner,
                .failReason = fail_reason,
                .duration = duration,
                .callGUID = call_guid
            };
            if (type == 30) {
                open_events.push_back(event_data);
            } else {
                int index = -1;
                for (size_t i = 0; i < open_events.size(); i++) {
                    if (open_events.at(i).matches(convo_id, dialog_partner, call_guid)) {
                        invariant(index == -1, "Multiple start call events match");
                        index = i;
                    }
                }
                invariant(index != -1, "No start call event matches");

                create_call_events(prescanned_call_events, open_events.at(index), event_data, raw_calls);

                open_events.erase(open_events.begin() + index);
            }
        });

    invariant(open_events.empty(), "Not all start call events were matched!");

    return prescanned_call_events;
}

static void create_call_events(
    map<uint64_t, unique_ptr<RawEvent>>& mut_prescanned_call_events,
    IMM(CallEventData) start_event_data,
    IMM(CallEventData) end_event_data,
    const map<uint64_t, RawSkypeCall>& raw_calls
) {
    uint64_t adjusted_start_timestamp = start_event_data.timestamp;
    if (start_event_data.duration && (start_event_data.timestamp == end_event_data.timestamp)) {
        adjusted_start_timestamp = end_event_data.timestamp - *start_event_data.duration;
    }

    CPTR(RawSkypeCall) skype_call = find_corresponding_call(
        raw_calls,
        adjusted_start_timestamp,
        end_event_data.timestamp,
        start_event_data.convoID
    );

    unique_ptr<ApparentSubject> initiator = make_unique<FullySpecifiedSubject>(
       parse_skype_account(start_event_data.authorAccount),
       start_event_data.authorScreenName
    );
    unique_ptr<ApparentSubject> ender = make_unique<FullySpecifiedSubject>(
        parse_skype_account(end_event_data.authorAccount),
        end_event_data.authorScreenName
    );

    auto peers = get_call_event_peers(start_event_data, end_event_data, skype_call);

    auto start_event = make_unique<RawStartCallEvent>(
        ApparentTime::fromUnixTimestamp(adjusted_start_timestamp),
        0,
        move(initiator),
        move(peers)
    );
    auto end_event = make_unique<RawEndCallEvent>(
        ApparentTime::fromUnixTimestamp(end_event_data.timestamp),
        0
    );
    end_event->ender = move(ender);

    if (start_event_data.callGUID) {
        start_event->skypeCallGUID = *start_event_data.callGUID;
        end_event->skypeCallGUID = *start_event_data.callGUID;
    } else {
        QString synthetic_guid = QString("%1:%2").arg(start_event_data.dialogPartner).arg(adjusted_start_timestamp);
        start_event->syntheticCallGUID = synthetic_guid;
        end_event->syntheticCallGUID = synthetic_guid;
    }
    if (skype_call) {
        start_event->correspondingSkypeCallID = skype_call->id;
        end_event->correspondingSkypeCallID = skype_call->id;
    }

    uint actual_duration = end_event_data.timestamp - adjusted_start_timestamp;
    if (actual_duration >= 5) {
        start_event->durationSeconds = actual_duration;

        if (!start_event_data.failReason.isEmpty()) {
            end_event->reasonCallFailed = parse_current_call_fail_reason(start_event_data.failReason);
        }
    } else {
        start_event->reasonFailed = parse_start_call_fail_reason(start_event_data.failReason);

        // Absorb the end call event if the call failed
        end_event.reset();
    }

    mut_prescanned_call_events[start_event_data.eventID] = move(start_event);
    mut_prescanned_call_events[end_event_data.eventID] = move(end_event);
}

static inline uint64_t abs_difference(uint64_t a, uint64_t b) {
    return (b > a) ? (b - a) : (a - b);
}

static CPTR(RawSkypeCall) find_corresponding_call(
    const map<uint64_t, RawSkypeCall>& raw_calls,
    uint64_t start_timestamp,
    uint64_t end_timestamp,
    uint64_t convo_id
) {
    invariant(start_timestamp <= end_timestamp, "start_timestamp must be less than end_timestamp");
    uint64_t actual_duration = end_timestamp - start_timestamp;

    CPTR(RawSkypeCall) closest_call = nullptr;

    for (IMM(auto) kv : raw_calls) {
        if (
            (kv.second.convDBID && (kv.second.convDBID != convo_id)) ||
            (abs_difference(kv.second.duration ? *kv.second.duration : 0, actual_duration) > 50) ||
            (abs_difference(kv.second.beginTimestamp, start_timestamp) > 7200)
        ) {
            continue;
        }

        if (!closest_call || (
            abs_difference(start_timestamp, kv.second.beginTimestamp) <
            abs_difference(start_timestamp, closest_call->beginTimestamp)
        )) {
            closest_call = &raw_calls.at(kv.first);
        }
    }

    return closest_call;
}

static vector<CEDE(ApparentSubject)> get_call_event_peers(
   IMM(CallEventData) start_event_data,
   IMM(CallEventData) end_event_data,
   CPTR(RawSkypeCall) skype_call
) {
    // This is a complete mess. Peers may or may not appear in any of these places depending on the Skype version,
    // whether the call failed or not, etc. We gather all of the intended peers on a best-effort basis.

    map<QString, QString> peers;

    if (!start_event_data.identities.isEmpty()) {
        for (IMM(QString) account : start_event_data.identities.split(" ")) {
            peers[account] = "";
        }
    }

    if (skype_call) {
        for (IMM(auto) kv : skype_call->participants) {
            peers[kv.first] = kv.second;
        }
    }

    QDomDocument xml = xml_from_string(start_event_data.bodyXML);
    QDomElement root = get_dom_root(xml, "partlist");

    for (
        QDomElement part_elem = root.firstChildElement();
        !part_elem.isNull();
        part_elem = part_elem.nextSiblingElement()
    ) {
        invariant(part_elem.tagName() == "part", "Expected <partlist> to have <part> children");
        peers[read_string_attr(part_elem, "identity")] = read_text_only_content(child_elem(part_elem, "name"));
    }

    peers[end_event_data.authorAccount] = end_event_data.authorScreenName;

    peers.erase(start_event_data.authorAccount);

    vector<unique_ptr<ApparentSubject>> peers_vector;

    for (IMM(auto) kv : peers) {
        peers_vector.push_back(make_unique<FullySpecifiedSubject>(parse_skype_account(kv.first), kv.second));
    }

    return peers_vector;
}

static StartCallFailReason parse_start_call_fail_reason(IMM(QString) raw_reason) {
    static const map<QString, StartCallFailReason> lookup {
        { "", StartCallFailReason::UNDETERMINED },
        { "no_answer", StartCallFailReason::NO_ANSWER },
        { "busy", StartCallFailReason::PEER_BUSY },
        { "blocked_by_privacy_settings", StartCallFailReason::PRIVACY_BLOCKED },
        { "manual", StartCallFailReason::CALL_REJECTED },
        { "insufficient_funds", StartCallFailReason::INSUFFICIENT_FUNDS },
    };

    invariant(lookup.count(raw_reason), "Could not parse call failure reason: %s", QP(raw_reason));

    return lookup.at(raw_reason);
}

static CurrentCallFailReason parse_current_call_fail_reason(IMM(QString) raw_reason) {
    static const map<QString, CurrentCallFailReason> lookup {
        { "connection_dropped", CurrentCallFailReason::CONNECTION_DROPPED }
    };

    invariant(lookup.count(raw_reason), "Could not parse mid-call failure reason: %s", QP(raw_reason));

    return lookup.at(raw_reason);
}

}}}
