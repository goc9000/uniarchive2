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
#include "utils/language/shortcuts.h"

#include <QString>

namespace uniarchive2 { namespace extraction { namespace adium {

using namespace uniarchive2::intermediate_format;

RawConversation extract_adium_conversation(IMM(QString) filename);

}}}

#endif //UNIARCHIVE2_EXTRACTION_ADIUM_EXTRACT_ADIUM_CONVERSATIONS_H
