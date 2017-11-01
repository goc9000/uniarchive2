/**
 * extraction/extract_conversations_generic.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_EXTRACT_CONVERSATIONS_GENERIC_H
#define UNIARCHIVE2_EXTRACTION_EXTRACT_CONVERSATIONS_GENERIC_H

#include "intermediate_format/RawConversationCollection.h"
#include "sources/atomic/AtomicConversationSource.h"
#include "protocols/ArchiveFormat.h"

#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace extraction {

using namespace uniarchive2::intermediate_format;
using namespace uniarchive2::protocols;
using namespace uniarchive2::sources;

RawConversationCollection extract_conversations_generic(ArchiveFormat format, IMM(AtomicConversationSource) source);

}}

#endif //UNIARCHIVE2_EXTRACTION_EXTRACT_CONVERSATIONS_GENERIC_H
