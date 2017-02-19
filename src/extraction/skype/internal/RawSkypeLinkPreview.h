/**
 * extraction/skype/internal/RawSkypeLinkPreview.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPELINKPREVIEW_H
#define UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPELINKPREVIEW_H

#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <QString>

namespace uniarchive2 { namespace extraction { namespace skype { namespace internal {

using namespace std;

struct RawSkypeLinkPreview {
    QString uri;
    optional<QString> title;
    optional<QString> description;
    optional<QString> service;
    QString mime_type;
    QString skype_type;

    RawSkypeLinkPreview(
        IMM(QString) uri,
        optional<QString> title,
        optional<QString> description,
        optional<QString> service,
        QString mime_type,
        QString skype_type
    ) : uri(uri), title(title), description(description), service(service), mime_type(mime_type),
        skype_type(skype_type) {
        // Nothing else to initialize
    }
};

}}}}

#endif //UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPELINKPREVIEW_H
