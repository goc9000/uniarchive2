/**
 * extraction/pidgin/extract_pidgin_conversations.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_PIDGIN_EXTRACT_PIDGIN_CONVERSATIONS_H
#define UNIARCHIVE2_EXTRACTION_PIDGIN_EXTRACT_PIDGIN_CONVERSATIONS_H

#include <QString>

#include "intermediate_format/RawConversation.h"
#include "utils/language/shortcuts.h"

using namespace uniarchive2::intermediate_format;

namespace uniarchive2 { namespace extraction { namespace pidgin {

RawConversation extract_pidgin_html_conversation(IMM(QString) filename);

}}}

#endif //UNIARCHIVE2_EXTRACTION_PIDGIN_EXTRACT_PIDGIN_CONVERSATIONS_H
