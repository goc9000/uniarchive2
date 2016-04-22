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

QString name_for_archive_format(const ArchiveFormats& protocol) {
    switch (protocol) {
        case ArchiveFormats::INVALID:
            return "(invalid)";
        case ArchiveFormats::YAHOO_MESSENGER:
            return "Yahoo Messenger";
        case ArchiveFormats::MSN_MESSENGER_XML:
            return "MSN Messenger (XML format)";
    }
}

QDebug operator<< (QDebug stream, const ArchiveFormats& format) {
    stream << qPrintable(name_for_archive_format(format));

    return stream;
}

}}
