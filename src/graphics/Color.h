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

namespace uniarchive2 { namespace graphics {

struct Color {
public:
    unsigned char r;
    unsigned char g;
    unsigned char b;

    Color(unsigned char r, unsigned char g, unsigned char b);

    QString toHTMLFormat() const;

    static Color fromHTMLFormat(QString text);
};

QDebug operator<< (QDebug stream, const Color& color);

}}

#endif //UNIARCHIVE2_GRAPHICS_COLOR_H
