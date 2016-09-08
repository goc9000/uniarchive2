/**
 * intermediate_format/content/formatting/YahooAltTag.h
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
#include "intermediate_format/content/StandardTag.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::graphics;

using namespace std::experimental;

struct YahooAltTag : StandardTag {
public:
    optional<Color> color1;
    optional<Color> color2;

    YahooAltTag(IMM(Color) color1, IMM(Color) color2);
    YahooAltTag();

protected:
    virtual QString tagName() const;
    virtual void writeOpenTagAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_YAHOOALTTAG_H
