/**
 * extraction/skype/internal/RawSkypeIdentity.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPEIDENTITY_H
#define UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPEIDENTITY_H

#include "utils/language/shortcuts.h"

#include <QString>

namespace uniarchive2 { namespace extraction { namespace skype { namespace internal {

struct RawSkypeIdentity {
    QString accountName;
    QString screenName;

    RawSkypeIdentity(
        IMM(QString) account_name,
        IMM(QString) screen_name
    ) : accountName(account_name), screenName(screen_name) {}
};

}}}}

#endif //UNIARCHIVE2_EXTRACTION_SKYPE_INTERNAL_RAWSKYPEIDENTITY_H
