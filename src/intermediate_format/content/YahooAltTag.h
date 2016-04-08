/**
 * intermediate_format/content/YahooAltTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_YAHOOALTTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_YAHOOALTTAG_H

#include <QtDebug>

#include "graphics/Color.h"
#include "intermediate_format/content/Markup.h"
#include "utils/external_libs/optional.hpp"

using namespace uniarchive2::graphics;
using namespace std::experimental;

namespace uniarchive2 { namespace intermediate_format { namespace content {

struct YahooAltTag : Markup {
public:
    optional<Color> color1;
    optional<Color> color2;
    bool closed;

    YahooAltTag(bool closed, const Color& color1, const Color& color2);
    YahooAltTag(bool closed);
    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_YAHOOALTTAG_H
