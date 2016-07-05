/**
 * intermediate_format/content/formatting/FontTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_FONTTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_FONTTAG_H

#include "graphics/Color.h"
#include "intermediate_format/content/Markup.h"
#include "utils/external_libs/optional.hpp"

#include <QtDebug>
#include <QString>
#include <QList>

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace std::experimental;
using namespace uniarchive2::graphics;

struct FontTag : Markup {
public:
    QList<QString> faces;
    QString size;
    optional<Color> color;
    QString css;

    bool closed;

    FontTag(bool closed = false);
    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_FONTTAG_H
