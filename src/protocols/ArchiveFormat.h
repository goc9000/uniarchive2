/**
 * protocols/ArchiveFormat.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_PROTOCOLS_ARCHIVEFORMAT_H
#define UNIARCHIVE2_PROTOCOLS_ARCHIVEFORMAT_H

#include <QDebug>

namespace uniarchive2 { namespace protocols {

enum class ArchiveFormat {
    INVALID = 0,
    ADIUM,
    DIGSBY,
    FACEBOOK_DYI,
    MSN_MESSENGER_XML,
    YAHOO_MESSENGER_DAT,
};

QDebug operator<< (QDebug stream, ArchiveFormat format);

}}

#endif //UNIARCHIVE2_PROTOCOLS_ARCHIVEFORMAT_H
