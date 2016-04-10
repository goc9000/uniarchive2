/**
 * extraction/yahoo/extract_yahoo_messenger_conversations.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_YAHOO_EXTRACT_YAHOO_MESSENGER_CONVERSATIONS_H
#define UNIARCHIVE2_EXTRACTION_YAHOO_EXTRACT_YAHOO_MESSENGER_CONVERSATIONS_H

#include <QString>
#include <QList>

#include "intermediate_format/IntermediateFormatConversation.h"

using namespace uniarchive2::intermediate_format;

namespace uniarchive2 { namespace extraction { namespace yahoo {

QList<IntermediateFormatConversation> extract_yahoo_messenger_conversations(const QString& filename);

}}}

#endif //UNIARCHIVE2_EXTRACTION_YAHOO_EXTRACT_YAHOO_MESSENGER_CONVERSATIONS_H
