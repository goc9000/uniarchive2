/**
 * extraction/yahoo/extract_yahoo_messenger_dat_conversations.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_YAHOO_EXTRACT_YAHOO_MESSENGER_DAT_CONVERSATIONS_H
#define UNIARCHIVE2_EXTRACTION_YAHOO_EXTRACT_YAHOO_MESSENGER_DAT_CONVERSATIONS_H

#include <vector>

#include <QString>

#include "intermediate_format/IntermediateFormatConversation.h"
#include "utils/language/shortcuts.h"

using namespace std;
using namespace uniarchive2::intermediate_format;

namespace uniarchive2 { namespace extraction { namespace yahoo {

vector<IntermediateFormatConversation> extract_yahoo_messenger_dat_conversations(IMM(QString) filename);

}}}

#endif //UNIARCHIVE2_EXTRACTION_YAHOO_EXTRACT_YAHOO_MESSENGER_DAT_CONVERSATIONS_H
