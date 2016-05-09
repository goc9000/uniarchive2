/**
 * protocols/ArchiveFormats.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_PROTOCOLS_ARCHIVEFORMATS_H
#define UNIARCHIVE2_PROTOCOLS_ARCHIVEFORMATS_H

#include <QDebug>

namespace uniarchive2 { namespace protocols {

enum class ArchiveFormats {
    INVALID = 0,
    ADIUM,
    DIGSBY,
    FACEBOOK_DYI,
    MSN_MESSENGER_XML,
    YAHOO_MESSENGER_DAT,
};

QDebug operator<< (QDebug stream, ArchiveFormats format);

}}

#endif //UNIARCHIVE2_PROTOCOLS_ARCHIVEFORMATS_H
