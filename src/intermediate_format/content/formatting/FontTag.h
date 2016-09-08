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
#include "intermediate_format/content/StandardTag.h"
#include "utils/external_libs/optional.hpp"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::graphics;

using namespace std::experimental;

struct FontTag : StandardTag {
public:
    optional<vector<QString>> faces;
    optional<QString> size;
    optional<Color> color;
    optional<QString> css;

    FontTag(bool open);

protected:
    virtual QString tagName() const;
    virtual void writeOpenTagAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_FONTTAG_H
