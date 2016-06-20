/**
 * intermediate_format/content/LinkTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINKTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINKTAG_H

#include "intermediate_format/content/Markup.h"

#include <QtDebug>
#include <QUrl>

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct LinkTag : Markup {
public:
    QUrl url;
    bool closed;

    LinkTag(QUrl url);
    LinkTag(bool closed);

    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_LINKTAG_H
