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

#include <QtDebug>
#include <QString>

#include "intermediate_format/content/IntermediateFormatMessageContentItem.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct TextSection : IntermediateFormatMessageContentItem {
public:
    QString text;

    TextSection(IMM(QString) text);
    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_TEXTSECTION_H
