/**
 * extraction/pidgin/extract_pidgin_txt_conversations.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>

#include "extraction/pidgin/extract_pidgin_txt_conversations.h"
#include "extraction/pidgin/common_extract_pidgin_conversations_code.h"
#include "intermediate_format/subjects/SubjectGivenAsAccount.h"
#include "intermediate_format/ApparentTime.h"
#include "protocols/FullAccountName.h"
#include "utils/external_libs/make_unique.hpp"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"
#include "utils/time/parse_date_parts.h"

using namespace std;
using namespace uniarchive2::extraction;
using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::intermediate_format::events;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;
using namespace uniarchive2::utils::time;

namespace uniarchive2 { namespace extraction { namespace pidgin {

RawConversation extract_pidgin_txt_conversation(IMM(QString) filename) {
    RawConversation conversation = init_conversation(filename, "txt", ArchiveFormat::PIDGIN_TXT);

    return conversation;
}

}}}
