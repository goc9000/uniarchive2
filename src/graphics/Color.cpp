/**
 * graphics/Color.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QChar>
#include <QDebugStateSaver>
#include <QRegularExpression>

#include "graphics/Color.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace graphics {

Color::Color(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {
}

QString Color::toHTMLFormat() const {
    return QString("#%1%2%3").arg(r, 2, 16, QChar('0')).arg(g, 2, 16, QChar('0')).arg(b, 2, 16, QChar('0'));
}

Color Color::fromHTMLFormat(IMM(QString) text) {
    static QRegularExpression pattern(
        "^\\s*#?(?<r>[0-9a-f]{2})(?<g>[0-9a-f]{2})(?<b>[0-9a-f]{2})\\s*$",
        QRegularExpression::CaseInsensitiveOption
    );
    auto match = pattern.match(text);

    invariant(match.hasMatch(), "Invalid HTML color format: \"%s\"", QP(text));

    return Color(
        (unsigned char)match.captured("r").toInt(nullptr, 16),
        (unsigned char)match.captured("g").toInt(nullptr, 16),
        (unsigned char)match.captured("b").toInt(nullptr, 16)
    );
}

QDebug operator<< (QDebug stream, IMM(Color) color) {
    stream << QP(color.toHTMLFormat());

    return stream;
}

}}
