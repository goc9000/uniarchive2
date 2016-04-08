/**
 * intermediate_format/content/YahooFadeTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_YAHOOFADETAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_YAHOOFADETAG_H

#include <QtDebug>
#include <QList>

#include "graphics/Color.h"
#include "intermediate_format/content/Markup.h"

using namespace uniarchive2::graphics;

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct YahooFadeTag : Markup {
public:
    QList<Color> colors;
    bool closed;

    YahooFadeTag(bool closed, const QList<Color>& colors = QList<Color>());
    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_YAHOOFADETAG_H
