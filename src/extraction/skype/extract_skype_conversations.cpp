/**
 * extraction/skype/extract_skype_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>

#include "extraction/skype/extract_skype_conversations.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/sqlite/SQLiteDB.h"

using namespace std;
using namespace uniarchive2::utils::sqlite;

namespace uniarchive2 { namespace extraction { namespace skype {

vector<RawConversation> extract_skype_conversations(IMM(QString) filename) {
    vector<RawConversation> conversations;

    SQLiteDB db = SQLiteDB::openReadOnly(filename);

    return conversations;
}

}}}
