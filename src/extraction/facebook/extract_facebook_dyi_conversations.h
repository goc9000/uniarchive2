/**
 * extraction/facebook/extract_facebook_dyi_conversations.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_FACEBOOK_EXTRACT_FACEBOOK_DYI_CONVERSATIONS_H
#define UNIARCHIVE2_EXTRACTION_FACEBOOK_EXTRACT_FACEBOOK_DYI_CONVERSATIONS_H

#include "intermediate_format/RawConversation.h"
#include "utils/language/shortcuts.h"

#include <vector>

#include <QString>

namespace uniarchive2 { namespace extraction { namespace facebook {

using namespace std;
using namespace uniarchive2::intermediate_format;

vector<RawConversation> extract_facebook_dyi_conversations(IMM(QString) filename);

}}}

#endif //UNIARCHIVE2_EXTRACTION_FACEBOOK_EXTRACT_FACEBOOK_DYI_CONVERSATIONS_H
