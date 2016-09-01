/**
 * intermediate_format/content/symbols/FlagIcon.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_FLAGICON_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_FLAGICON_H

#include "intermediate_format/content/RawMessageContentItem.h"
#include "utils/language/shortcuts.h"

#include <QtDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct FlagIcon : RawMessageContentItem {
public:
    QString countryCode;
    QString textEquivalent;

    FlagIcon(IMM(QString) country_code, IMM(QString) text_equivalent);

protected:
    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_FLAGICON_H
