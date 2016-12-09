/**
 * graphics/Color.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_GRAPHICS_COLOR_H
#define UNIARCHIVE2_GRAPHICS_COLOR_H

#include <QDebug>
#include <QString>

#include "utils/language/shortcuts.h"
#include "utils/serialization/ISerializable.h"

namespace uniarchive2 { namespace graphics {

using namespace uniarchive2::utils::serialization;

struct Color : ISerializable {
public:
    uint8_t r;
    uint8_t g;
    uint8_t b;

    Color(uint8_t r, uint8_t g, uint8_t b);

    QString toHTMLFormat() const;

    static Color fromHTMLFormat(IMM(QString) text);

protected:
    void serializeToStream(QDataStream& mut_stream) const;
};

QDebug operator<< (QDebug stream, IMM(Color) color);

}}

#endif //UNIARCHIVE2_GRAPHICS_COLOR_H
