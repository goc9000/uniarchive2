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

#include "graphics/Color.h"
#include "utils/language/invariant.h"
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace graphics {

Color::Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {
}

QString Color::toHTMLFormat() const {
    return QString("#%1%2%3").arg(r, 2, 16, QChar('0')).arg(g, 2, 16, QChar('0')).arg(b, 2, 16, QChar('0'));
}

Color Color::fromHTMLFormat(IMM(QString) text) {
    QREGEX_MUST_MATCH_CI(
        match, "^\\s*#?(?<r>[0-9a-f]{2})(?<g>[0-9a-f]{2})(?<b>[0-9a-f]{2})\\s*$", text,
        "Invalid HTML color format: \"%s\""
    );

    return Color(
        (uint8_t)match.captured("r").toInt(nullptr, 16),
        (uint8_t)match.captured("g").toInt(nullptr, 16),
        (uint8_t)match.captured("b").toInt(nullptr, 16)
    );
}

QDebug operator<< (QDebug stream, IMM(Color) color) {
    stream << QP(color.toHTMLFormat());

    return stream;
}

}}
