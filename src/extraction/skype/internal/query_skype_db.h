/**
 * extraction/skype/internal/query_skype_db.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_QUERY_SKYPE_DB_H
#define UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_QUERY_SKYPE_DB_H

#include "extraction/skype/internal/RawSkypeIdentity.h"
#include "extraction/skype/internal/RawSkypeConvo.h"
#include "extraction/skype/internal/RawSkypeChat.h"
#include "extraction/skype/internal/RawSkypeCall.h"
#include "utils/sqlite/SQLiteDB.h"

#include <QString>

#include <map>

namespace uniarchive2 { namespace extraction { namespace skype { namespace internal {

using namespace std;

using namespace uniarchive2::utils::sqlite;

map<QString, RawSkypeIdentity> query_raw_skype_identities(SQLiteDB &db);
map<uint64_t, RawSkypeConvo> query_raw_skype_convos(SQLiteDB &db);
map<QString, RawSkypeChat> query_raw_skype_chats(SQLiteDB &db);
map<uint64_t, RawSkypeCall> query_raw_skype_calls(SQLiteDB &db);

}}}}

#endif //UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_QUERY_SKYPE_DB_H
