/**
 * intermediate_format/content/TextSection.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_TEXTSECTION_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_TEXTSECTION_H

#include "intermediate_format/content/RawMessageContentItem.h"
#include "utils/language/shortcuts.h"

#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct TextSection : RawMessageContentItem {
public:
    QString text;

    TextSection(IMM(QString) text);

protected:
    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_TEXTSECTION_H
