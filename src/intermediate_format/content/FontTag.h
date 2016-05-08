/**
 * intermediate_format/content/FontTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_FONTTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_FONTTAG_H

#include <QtDebug>
#include <QString>
#include <QList>

#include "graphics/Color.h"
#include "intermediate_format/content/Markup.h"
#include "utils/external_libs/optional.hpp"

using namespace uniarchive2::graphics;

using namespace std::experimental;

namespace uniarchive2 { namespace intermediate_format { namespace content {

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
