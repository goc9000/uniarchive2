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

#include <vector>

#include <QString>

#include "intermediate_format/IntermediateFormatConversation.h"

using namespace std;
using namespace uniarchive2::intermediate_format;

namespace uniarchive2 { namespace extraction { namespace facebook {

vector<IntermediateFormatConversation> extract_facebook_dyi_conversations(const QString& filename);

}}}

#endif //UNIARCHIVE2_EXTRACTION_FACEBOOK_EXTRACT_FACEBOOK_DYI_CONVERSATIONS_H
