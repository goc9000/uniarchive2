/**
 * graphics/ANSIColor.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 75cb5b1d57d8d7190ae36466e1fa8a165dd754b7e729534740128735a5fbd906
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_GRAPHICS_ANSICOLOR_H
#define UNIARCHIVE2_GRAPHICS_ANSICOLOR_H

#include <QDataStream>
#include <QDebug>
#include <QString>

namespace uniarchive2 { namespace graphics {

enum class ANSIColor : uint8_t {
    BLACK = 0,
    RED = 1,
    GREEN = 2,
    YELLOW = 3,
    BLUE = 4,
    MAGENTA = 5,
    CYAN = 6,
    WHITE = 7,
    EXTENDED = 8,
};

QString name_for_ansi_color(ANSIColor color);

QDebug operator<< (QDebug stream, ANSIColor color);

QDataStream& operator>> (QDataStream& mut_stream, ANSIColor& mut_color);
QDataStream& operator<< (QDataStream& mut_stream, ANSIColor color);

}}

#endif //UNIARCHIVE2_GRAPHICS_ANSICOLOR_H
