/**
 * intermediate_format/content/CSSStyleTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_CSSSTYLETAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_CSSSTYLETAG_H

#include <QtDebug>
#include <QString>

#include "intermediate_format/content/Markup.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct CSSStyleTag : Markup {
public:
    QString css;
    bool closed;

    CSSStyleTag(IMM(QString) css);
    CSSStyleTag(bool closed);
    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_CSSSTYLETAG_H
