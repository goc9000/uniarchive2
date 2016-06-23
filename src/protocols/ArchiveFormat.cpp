/**
 * protocols/ArchiveFormat.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "protocols/ArchiveFormat.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace protocols {

QString name_for_archive_format(ArchiveFormat protocol) {
    switch (protocol) {
        case ArchiveFormat::INVALID:
            return "(invalid)";
        case ArchiveFormat::ADIUM:
            return "Adium";
        case ArchiveFormat::DIGSBY:
            return "Digsby";
        case ArchiveFormat::FACEBOOK_DYI:
            return "Facebook (DYI archive)";
        case ArchiveFormat::MSN_MESSENGER_XML:
            return "MSN Messenger (XML format)";
        case ArchiveFormat::PIDGIN_HTML:
            return "Pidgin (HTML format)";
        case ArchiveFormat::PIDGIN_TXT:
            return "Pidgin (TXT format)";
        case ArchiveFormat::SKYPE:
            return "Skype";
        case ArchiveFormat::WHATSAPP_EMAIL:
            return "WhatsApp (Email format)";
        case ArchiveFormat::YAHOO_MESSENGER_DAT:
            return "Yahoo Messenger (DAT format)";
    }
}

QDebug operator<< (QDebug stream, ArchiveFormat format) {
    stream << QP(name_for_archive_format(format));

    return stream;
}

}}
