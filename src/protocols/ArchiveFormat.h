/**
 * protocols/ArchiveFormat.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): c9bfc72405ac9a6337fe5e3ad746e6968c950fccb469ca74a9e158c289a75bae
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_PROTOCOLS_ARCHIVEFORMAT_H
#define UNIARCHIVE2_PROTOCOLS_ARCHIVEFORMAT_H

#include <QDataStream>
#include <QDebug>
#include <QString>

namespace uniarchive2 { namespace protocols {

enum class ArchiveFormat {
    INVALID = 0,
    ADIUM,
    DIGSBY,
    FACEBOOK_DYI,
    MSN_MESSENGER_XML,
    PIDGIN_HTML,
    PIDGIN_TXT,
    SKYPE,
    WHATSAPP_EMAIL,
    YAHOO_MESSENGER_DAT,
};

QString name_for_archive_format(ArchiveFormat format);

QDebug operator<< (QDebug stream, ArchiveFormat format);
QDataStream& operator<< (QDataStream& mut_stream, ArchiveFormat format);

}}

#endif //UNIARCHIVE2_PROTOCOLS_ARCHIVEFORMAT_H
