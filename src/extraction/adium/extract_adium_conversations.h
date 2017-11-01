/**
 * extraction/adium/extract_adium_conversations.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_ADIUM_EXTRACT_ADIUM_CONVERSATIONS_H
#define UNIARCHIVE2_EXTRACTION_ADIUM_EXTRACT_ADIUM_CONVERSATIONS_H

#include "intermediate_format/RawConversation.h"
#include "sources/atomic/AtomicConversationSource.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace extraction { namespace adium {

using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::sources;

RawConversation extract_adium_conversation(IMM(AtomicConversationSource) source);

}}}

#endif //UNIARCHIVE2_EXTRACTION_ADIUM_EXTRACT_ADIUM_CONVERSATIONS_H
