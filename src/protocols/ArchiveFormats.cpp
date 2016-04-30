/**
 * protocols/ArchiveFormats.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>

#include "protocols/ArchiveFormats.h"

namespace uniarchive2 { namespace protocols {

QString name_for_archive_format(ArchiveFormats protocol) {
    switch (protocol) {
        case ArchiveFormats::INVALID:
            return "(invalid)";
        case ArchiveFormats::YAHOO_MESSENGER_DAT:
            return "Yahoo Messenger (DAT format)";
        case ArchiveFormats::MSN_MESSENGER_XML:
            return "MSN Messenger (XML format)";
        case ArchiveFormats::FACEBOOK_DYI:
            return "Facebook (DYI archive)";
    }
}

QDebug operator<< (QDebug stream, ArchiveFormats format) {
    stream << qPrintable(name_for_archive_format(format));

    return stream;
}

}}
