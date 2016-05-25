/**
 * extraction/pidgin/common_extract_pidgin_txt_conversations_code.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_PIDGIN_COMMON_EXTRACT_PIDGIN_CONVERSATIONS_CODE_H
#define UNIARCHIVE2_EXTRACTION_PIDGIN_COMMON_EXTRACT_PIDGIN_CONVERSATIONS_CODE_H

#include <QString>

#include "intermediate_format/ApparentTime.h"
#include "intermediate_format/RawConversation.h"
#include "protocols/ArchiveFormat.h"
#include "utils/language/shortcuts.h"

using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::protocols;

namespace uniarchive2 { namespace extraction { namespace pidgin {

RawConversation init_conversation(IMM(QString)filename, IMM(QString) expected_extension, ArchiveFormat format);
ApparentTime parse_timestamp(IMM(QString) timestamp_text, IMM(RawConversation) conversation);

}}}

#endif //UNIARCHIVE2_EXTRACTION_PIDGIN_COMMON_EXTRACT_PIDGIN_CONVERSATIONS_CODE_H
