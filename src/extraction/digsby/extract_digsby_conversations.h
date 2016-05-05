/**
 * extraction/digsby/extract_digsby_conversations.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_DIGSBY_EXTRACT_DIGSBY_CONVERSATIONS_H
#define UNIARCHIVE2_EXTRACTION_DIGSBY_EXTRACT_DIGSBY_CONVERSATIONS_H

#include <QString>

#include "intermediate_format/IntermediateFormatConversation.h"
#include "utils/language/shortcuts.h"

using namespace uniarchive2::intermediate_format;

namespace uniarchive2 { namespace extraction { namespace digsby {

IntermediateFormatConversation extract_digsby_conversation(IMM(QString) filename);

}}}

#endif //UNIARCHIVE2_EXTRACTION_DIGSBY_EXTRACT_DIGSBY_CONVERSATIONS_H
