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
#include "intermediate_format/provenance/ArchiveFileProvenance.h"
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

#include <set>
#include <map>

#include <QtDebug>
#include <QFileInfo>

namespace uniarchive2 { namespace extraction { namespace skype {

using namespace std;
using namespace uniarchive2::extraction::skype::internal;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::provenance;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;
using namespace uniarchive2::protocols::skype;
using namespace uniarchive2::utils::sqlite;

static RawConversation init_prototype(IMM(QString) filename);

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
    IMM(RawConversation) prototype
);
static RawConversation convert_one_on_one_conversation(
    IMM(RawSkypeConvo) skype_convo,
    CPTR(RawSkypeChat) skype_chat,
    IMM(RawConversation) prototype,
    const map<QString, RawSkypeIdentity>& raw_identities
);
static RawConversation convert_group_chat(
    IMM(RawSkypeConvo) skype_convo,
    CPTR(RawSkypeChat) skype_chat,
    IMM(RawConversation) prototype,
    const map<QString, RawSkypeIdentity>& raw_identities
);
static CEDE(ApparentSubject) extract_identity_from_participants(
    set<QString>& mut_participants,
    const map<QString, RawSkypeIdentity>& raw_identities
);


vector<RawConversation> extract_skype_conversations(IMM(QString) filename) {
    SQLiteDB db = SQLiteDB::openReadOnly(filename);

    RawConversation prototype = init_prototype(filename);

    map<QString, RawSkypeIdentity> raw_identities = query_raw_skype_identities(db);
    map<uint64_t, RawSkypeConvo> raw_convos = query_raw_skype_convos(db);
    map<QString, RawSkypeChat> raw_chats = query_raw_skype_chats(db);
    map<uint64_t, RawSkypeCall> raw_calls = query_raw_skype_calls(db);

    map<QString, RawConversation> indexed_conversations = convert_conversations(
        db,
        raw_identities,
        raw_convos,
        raw_chats,
        raw_calls,
        prototype
    );

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
    IMM(RawConversation) prototype
) {
    map<QString, RawConversation> conversations;

    db.stmt(
        "SELECT chatname, convo_id FROM Messages "\
        "GROUP BY chatname, convo_id"
    ).forEachRow(
        [&conversations, &raw_convos, &raw_chats, &raw_calls, &raw_identities, &prototype](
            QString chat_string_id,
            uint64_t convo_id
        ) -> void {
            QString key = chat_string_id.isEmpty() ? QString::number(convo_id) : chat_string_id;
            invariant(!conversations.count(key), "Multiple conversations map to key: %s", QP(key));

            IMM(RawSkypeConvo) convo = raw_convos.at(convo_id);
            CPTR(RawSkypeChat) chat = raw_chats.count(chat_string_id) ? &raw_chats.at(chat_string_id) : nullptr;

            switch (convo.type) {
                case RawSkypeConvo::Type::ONE_ON_ONE:
                    if (chat && chat->type != RawSkypeChat::Type::ONE_ON_ONE) {
                        // Some group chats start with a (spurious) association with a one-on-one convo. Ignore this.
                        return;
                    }

                    conversations.emplace(key, convert_one_on_one_conversation(convo, chat, prototype, raw_identities));
                    return;
                case RawSkypeConvo::Type::GROUP_CHAT:
                    conversations.emplace(key, convert_group_chat(convo, chat, prototype, raw_identities));
                    return;
                default:
                    return;
            }
        });

    return conversations;
}

static RawConversation convert_one_on_one_conversation(
    IMM(RawSkypeConvo) skype_convo,
    CPTR(RawSkypeChat) skype_chat,
    IMM(RawConversation) prototype,
    const map<QString, RawSkypeIdentity>& raw_identities
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

    RawConversation conversation = RawConversation::fromPrototype(prototype);
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

    return conversation;
}

static RawConversation convert_group_chat(
    IMM(RawSkypeConvo) skype_convo,
    CPTR(RawSkypeChat) skype_chat,
    IMM(RawConversation) prototype,
    const map<QString, RawSkypeIdentity>& raw_identities
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

    RawConversation conversation = RawConversation::fromPrototype(prototype);
    conversation.isConference = true;
    conversation.identity = extract_identity_from_participants(participants, raw_identities);

    if (skype_convo.creator) {
        QString initiator = *skype_convo.creator;
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

}}}
