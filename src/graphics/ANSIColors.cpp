/**
 * graphics/ANSIColors.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>

#include "graphics/ANSIColors.h"

namespace uniarchive2 { namespace graphics {

QString name_for_color(ANSIColors color) {
    switch (color) {
        case ANSIColors::BLACK:
            return "black";
        case ANSIColors::RED:
            return "red";
        case ANSIColors::GREEN:
            return "green";
        case ANSIColors::YELLOW:
            return "yellow";
        case ANSIColors::BLUE:
            return "blue";
        case ANSIColors::MAGENTA:
            return "magenta";
        case ANSIColors::CYAN:
            return "cyan";
        case ANSIColors::WHITE:
            return "white";
        case ANSIColors::EXTENDED:
            return "extended";
    }
}

QDebug operator<< (QDebug stream, ANSIColors color) {
    stream << qPrintable(name_for_color(color));

    return stream;
}

}}
