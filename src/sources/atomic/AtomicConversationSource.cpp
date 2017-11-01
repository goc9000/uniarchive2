/**
 * sources/atomic/AtomicConversationSource.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "sources/atomic/AtomicConversationSource.h"

#include "utils/text/decoding.h"

namespace uniarchive2 { namespace sources {

using namespace uniarchive2::utils::text;

QString AtomicConversationSource::fullUTF8Text() const {
    return decode_utf8(fullData());
}

}}
