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
#include "utils/xml/load_xml.h"

#include <QDir>

namespace uniarchive2 { namespace sources {

using namespace uniarchive2::utils::text;
using namespace uniarchive2::utils::xml;

QString AtomicConversationSource::logicalFilename() const {
    return logicalFilenameSection(-1);
}

QString AtomicConversationSource::logicalFilenameSection(int index) const {
    return logicalFullFilename().section('/', index, index);
}

QString AtomicConversationSource::logicalFilenameSections(int from, int to) const {
    return logicalFullFilename().section('/', from, to);
}

QString AtomicConversationSource::fullUTF8Text() const {
    return decode_utf8(fullData());
}

QDomDocument AtomicConversationSource::fullXML(bool keep_whitespace) const {
    return xml_from_raw_data(fullData(), keep_whitespace, debugName());
}

}}
