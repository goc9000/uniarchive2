/**
 * extraction/skype/internal/query_skype_db.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "extraction/skype/internal/query_skype_db.h"

#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace extraction { namespace skype { namespace internal {

map<QString, RawSkypeIdentity> query_raw_skype_identities(SQLiteDB& db) {
    map<QString, RawSkypeIdentity> identities = db.stmt(
        "SELECT skypename, fullname FROM Accounts"
    ).mapRowsToMap(
        [](QString account_name, QString screen_name) -> pair<QString, RawSkypeIdentity> {
            return make_pair(account_name, RawSkypeIdentity(account_name, screen_name));
        }
    );

    return identities;
}

map<uint64_t, RawSkypeConvo> query_raw_skype_convos(SQLiteDB& db) {
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

map<QString, RawSkypeChat> query_raw_skype_chats(SQLiteDB& db) {
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

map<uint64_t, RawSkypeCall> query_raw_skype_calls(SQLiteDB& db) {
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

}}}}
