/**
 * intermediate_format/content/formatting/YahooAltTag.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): bd0cfb124d3fdf314477ce2c54b12fc4504893870ce2aa998e020c1b44dcacda
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

using namespace std::experimental;
using namespace uniarchive2::graphics;

struct YahooAltTag : StandardTag {
public:
    optional<Color> color1;
    optional<Color> color2;

    YahooAltTag(IMM(Color) color1, IMM(Color) color2);
    YahooAltTag();

    virtual RawMessageContentItemSubType subType() const;

protected:
    virtual QString tagName() const;
    virtual void writeOpenTagAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_YAHOOALTTAG_H
