/**
 * extraction/msn/extract_msn_messenger_conversations.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_MSN_EXTRACT_MSN_MESSENGER_CONVERSATIONS_H
#define UNIARCHIVE2_EXTRACTION_MSN_EXTRACT_MSN_MESSENGER_CONVERSATIONS_H

#include <vector>

#include <QString>

#include "intermediate_format/IntermediateFormatConversation.h"

using namespace std;
using namespace uniarchive2::intermediate_format;

namespace uniarchive2 { namespace extraction { namespace msn {

vector<IntermediateFormatConversation> extract_msn_messenger_conversations(const QString& filename);

}}}

#endif //UNIARCHIVE2_EXTRACTION_MSN_EXTRACT_MSN_MESSENGER_CONVERSATIONS_H
