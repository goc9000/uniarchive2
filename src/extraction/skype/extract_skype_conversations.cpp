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
#include "intermediate_format/events/RawMessageEvent.h"
#include "intermediate_format/events/RawUninterpretedEvent.h"
#include "intermediate_format/provenance/ArchiveFileProvenance.h"
#include "intermediate_format/provenance/SkypeConversationProvenance.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "intermediate_format/subjects/FullySpecifiedSubject.h"
#include "intermediate_format/RawConversation.h"
#include "protocols/ArchiveFormat.h"
#include "protocols/IMProtocol.h"
#include "protocols/skype/account_name.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
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
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::provenance;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;
using namespace uniarchive2::protocols::skype;
using namespace uniarchive2::utils::sqlite;
using namespace uniarchive2::utils::xml;

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

static void convert_events(SQLiteDB& db, map<QString, RawConversation>& mut_indexed_conversations);
static CEDE(RawEvent) convert_event(
    IMM(ApparentTime) message_time,
    unsigned int message_index,
    int type,
    int chatmsg_type,
    IMM(QString) sender_account,
    IMM(QString) sender_screen_name,
    IMM(QString) body_xml,
    optional<QByteArray> skype_guid,
    optional<QString> edited_by,
    optional<uint64_t> edited_timestamp
);
static CEDE(RawMessageEvent) convert_message_event(
    IMM(ApparentTime) event_time,
    unsigned int event_index,
    TAKE(ApparentSubject) subject,
    IMM(QString) body_xml,
    optional<QString> edited_by,
    optional<uint64_t> edited_timestamp
);

static RawMessageContent parse_message_content(IMM(QString) content_xml);
static void parse_message_content_node(RawMessageContent& mut_content, IMM(QDomNode) node);
static void parse_message_content_element(RawMessageContent& mut_content, IMM(QDomElement) element);
static CEDE(SkypeQuote) parse_quote_element(IMM(QDomElement) element);


vector<RawConversation> extract_skype_conversations(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", QP(filename));

    unique_ptr<Provenance> base_provenance = ArchiveFileProvenance::fromQFileInfo(ArchiveFormat::SKYPE, file_info);

    SQLiteDB db = SQLiteDB::openReadOnly(filename);
    map<QString, RawSkypeIdentity> raw_identities = query_raw_skype_identities(db);
    map<uint64_t, RawSkypeConvo> raw_convos = query_raw_skype_convos(db);
    map<QString, RawSkypeChat> raw_chats = query_raw_skype_chats(db);
    map<uint64_t, RawSkypeCall> raw_calls = query_raw_skype_calls(db);

    map<QString, RawConversation> indexed_conversations =
        convert_conversations(db, raw_identities, raw_convos, raw_chats, raw_calls, base_provenance.get());

    convert_events(db, indexed_conversations);

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
            optional<unsigned int> duration,
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

static RawConversation init_prototype(IMM(QString) filename) {
    QFileInfo file_info(filename);
    invariant(file_info.exists(), "File does not exist: %s", QP(filename));

    RawConversation conversation(IMProtocol::SKYPE);
    conversation.provenance = ArchiveFileProvenance::fromQFileInfo(ArchiveFormat::SKYPE, file_info);

    return conversation;
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
        make_unique<SubjectGivenAsAccount>(parse_skype_account(skype_convo.identity))
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
        conversation.declaredInitiator = make_unique<SubjectGivenAsAccount>(parse_skype_account(initiator));

        if (participants.count(initiator)) {
            participants.erase(initiator);
            conversation.declaredPeers.emplace_back(
                make_unique<SubjectGivenAsAccount>(parse_skype_account(initiator))
            );
        }
    }
    for (IMM(QString) participant : participants) {
        conversation.declaredPeers.emplace_back(make_unique<SubjectGivenAsAccount>(parse_skype_account(participant)));
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

    return make_unique<SubjectGivenAsAccount>(parse_skype_account(account_name));
}

static void convert_events(SQLiteDB& db, map<QString, RawConversation>& mut_indexed_conversations) {
    db.stmt(
        "SELECT chatname, convo_id, timestamp, type, chatmsg_type, author, from_dispname, body_xml, "\
        "       guid, edited_by, edited_timestamp "\
        "FROM Messages "\
        "ORDER BY timestamp"
    ).forEachRow(
        [&mut_indexed_conversations](
            QString chat_string_id,
            uint64_t convo_id,
            uint64_t timestamp,
            int type,
            int chatmsg_type,
            QString author,
            QString from_dispname,
            QString body_xml,
            optional<QByteArray> skype_guid,
            optional<QString> edited_by,
            optional<uint64_t> edited_timestamp
        ) -> void {
            QString key = chat_string_id.isEmpty() ? QString::number(convo_id) : chat_string_id;
            RawConversation& mut_conversation = mut_indexed_conversations.at(key);

            ApparentTime event_time = ApparentTime::fromUnixTimestamp(timestamp);
            unsigned int event_index = mut_conversation.events.size();

            mut_conversation.events.push_back(convert_event(
                event_time,
                event_index,
                type,
                chatmsg_type,
                author,
                from_dispname,
                body_xml,
                skype_guid,
                edited_by,
                edited_timestamp
            ));
        });
}

static CEDE(RawEvent) convert_event(
    IMM(ApparentTime) event_time,
    unsigned int event_index,
    int type,
    int chatmsg_type,
    IMM(QString) sender_account,
    IMM(QString) sender_screen_name,
    IMM(QString) body_xml,
    optional<QByteArray> skype_guid,
    optional<QString> edited_by,
    optional<uint64_t> edited_timestamp
) {
    auto subject = make_unique<FullySpecifiedSubject>(parse_skype_account(sender_account), sender_screen_name);

    unique_ptr<RawEvent> event;

    if ((type == 61) && ((chatmsg_type == 3) || (chatmsg_type == 0))) {
        event = convert_message_event(
            event_time,
            event_index,
            move(subject),
            body_xml,
            edited_by,
            edited_timestamp
        );
    } else {
        // Default
        QByteArray data;

        event = make_unique<RawUninterpretedEvent>(event_time, event_index, data);
    }

    if (skype_guid) {
        event->skypeGUID = *skype_guid;
    }

    return event;
}

static CEDE(RawMessageEvent) convert_message_event(
    IMM(ApparentTime) event_time,
    unsigned int event_index,
    TAKE(ApparentSubject) subject,
    IMM(QString) body_xml,
    optional<QString> edited_by,
    optional<uint64_t> edited_timestamp
) {
    unique_ptr<RawMessageEvent> message = make_unique<RawMessageEvent>(
        event_time,
        event_index,
        move(subject),
        parse_message_content(body_xml)
    );

    if (edited_by) {
        invariant(edited_timestamp, "Edit timestamp not specified!");

        message->isEdited = true;
        message->editedBy = make_unique<SubjectGivenAsAccount>(parse_skype_account(*edited_by));
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

        mut_content.addItem(make_unique<LinkTag>(true));
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

}}}
