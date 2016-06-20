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

#include "graphics/Color.h"
#include "intermediate_format/content/Markup.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace std::experimental;
using namespace uniarchive2::graphics;

struct YahooAltTag : Markup {
public:
    optional<Color> color1;
    optional<Color> color2;
    bool closed;

    YahooAltTag(bool closed, IMM(Color) color1, IMM(Color) color2);
    YahooAltTag(bool closed);
    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_YAHOOALTTAG_H
