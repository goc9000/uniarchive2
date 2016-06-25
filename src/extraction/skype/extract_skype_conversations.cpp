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
#include "extraction/skype/internal/RawSkypeIdentity.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/sqlite/SQLiteDB.h"

#include <map>

#include <QtDebug>

namespace uniarchive2 { namespace extraction { namespace skype {

using namespace std;
using namespace uniarchive2::extraction::skype::internal;
using namespace uniarchive2::utils::sqlite;

static map<QString, RawSkypeIdentity> query_raw_skype_identities(SQLiteDB &db);
static map<uint64_t, RawSkypeConvo> query_raw_skype_convos(SQLiteDB &db);
static map<QString, RawSkypeChat> query_raw_skype_chats(SQLiteDB &db);


vector<RawConversation> extract_skype_conversations(IMM(QString) filename) {
    vector<RawConversation> conversations;

    SQLiteDB db = SQLiteDB::openReadOnly(filename);

    map<QString, RawSkypeIdentity> raw_identities = query_raw_skype_identities(db);
    map<uint64_t, RawSkypeConvo> raw_convos = query_raw_skype_convos(db);
    map<QString, RawSkypeChat> raw_chats = query_raw_skype_chats(db);

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

}}}
