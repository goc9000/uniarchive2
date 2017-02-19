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

map<QString, RawSkypeLinkPreview> query_skype_link_previews(SQLiteDB &db) {
    map<QString, RawSkypeLinkPreview> previews;

    bool table_exists = db.stmt("SELECT COUNT(*) FROM sqlite_master WHERE name=\"MediaDocuments\" AND type=\"table\"")
        .singleValue<bool>();

    if (!table_exists) {
        return previews;
    }

    db.stmt(
        "SELECT uri, type, title, description, service, mime_type, web_url, storage_document_id, original_name, "\
        "       message_id, convo_id "\
        "FROM MediaDocuments WHERE doc_type=6"
    ).forEachRow(
        [&previews](
            QString uri,
            QString type,
            optional<QString> title,
            optional<QString> description,
            optional<QString> service,
            optional<QString> mime_type,
            QString verify_web_url,
            optional<uint64_t> verify_storage_document_id,
            optional<QString> verify_original_name,
            uint64_t verify_message_id,
            uint64_t verify_convo_id
        ) {
            invariant(uri != "", "Empty URI for link preview");
            invariant(previews.count(uri) == 0, "Duplicate preview for URI: %s", QP(uri));

            invariant(verify_web_url == uri, "Expected web_url==uri for link preview");
            invariant(
                !verify_storage_document_id && !verify_original_name && !verify_message_id && !verify_convo_id,
                "Expected null storage_document_id, original_name, message_id, convo_id for link preview"
            );

            if (type == "UrlPreview.1") {
                // Handle failed link previews
                invariant(
                    !title && !description && !service && !mime_type,
                    "Failed link preview should have null title, description, service and mime_type"
                );

                return;
            }

            invariant(mime_type, "Successful link preview should have non-null mime_type");
            invariant(type.startsWith("UrlPreview.1/"), "Unexpected type for link preview: %s", QP(type));

            previews.emplace(uri, RawSkypeLinkPreview(uri, title, description, service, *mime_type, type));
        });

    return previews;
}

}}}}
