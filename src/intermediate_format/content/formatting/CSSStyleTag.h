/**
 * intermediate_format/content/formatting/CSSStyleTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_CSSSTYLETAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_CSSSTYLETAG_H

#include "intermediate_format/content/Markup.h"
#include "utils/language/shortcuts.h"

#include <QtDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct CSSStyleTag : Markup {
public:
    QString css;
    bool closed;

    CSSStyleTag(IMM(QString) css);
    CSSStyleTag(bool closed);

protected:
    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_CSSSTYLETAG_H
