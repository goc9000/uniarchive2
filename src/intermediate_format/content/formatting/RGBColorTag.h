/**
 * intermediate_format/content/formatting/RGBColorTag.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_RGBCOLORTAG_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_RGBCOLORTAG_H

#include "graphics/Color.h"
#include "intermediate_format/content/SymmetricTag.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace content {

using namespace uniarchive2::graphics;

struct RGBColorTag : SymmetricTag {
public:
    Color color;

    RGBColorTag(bool open, IMM(Color) color);

protected:
    virtual QString tagName() const;
    virtual void writeAttributesToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_CONTENT_RGBCOLORTAG_H
