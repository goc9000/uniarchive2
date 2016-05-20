/**
 * graphics/ANSIColor.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>

#include "graphics/ANSIColor.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace graphics {

QString name_for_color(ANSIColor color) {
    switch (color) {
        case ANSIColor::BLACK:
            return "black";
        case ANSIColor::RED:
            return "red";
        case ANSIColor::GREEN:
            return "green";
        case ANSIColor::YELLOW:
            return "yellow";
        case ANSIColor::BLUE:
            return "blue";
        case ANSIColor::MAGENTA:
            return "magenta";
        case ANSIColor::CYAN:
            return "cyan";
        case ANSIColor::WHITE:
            return "white";
        case ANSIColor::EXTENDED:
            return "extended";
    }
}

QDebug operator<< (QDebug stream, ANSIColor color) {
    stream << QP(name_for_color(color));

    return stream;
}

}}
