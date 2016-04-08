/**
 * graphics/ANSIColors.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_GRAPHICS_ANSICOLORS_H
#define UNIARCHIVE2_GRAPHICS_ANSICOLORS_H

#include <QDebug>

namespace uniarchive2 { namespace graphics {

enum class ANSIColors {
    BLACK = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    MAGENTA = 5,
    CYAN = 6,
    WHITE = 7,
    EXTENDED = 8
};

QDebug operator<< (QDebug stream, const ANSIColors& color);

}}

#endif //UNIARCHIVE2_GRAPHICS_ANSICOLORS_H
