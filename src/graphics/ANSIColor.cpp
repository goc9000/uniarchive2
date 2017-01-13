/**
 * graphics/ANSIColor.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): ed9376c8d0789b1d23f970daa8c01efe36b8bf93fdec9c9de1f4574a6bb75e06
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "graphics/ANSIColor.h"
#include "utils/qt/shortcuts.h"
#include "utils/serialization/deserialization_helpers.h"

#include <QMap>
#include <QtDebug>

namespace uniarchive2 { namespace graphics {

using namespace uniarchive2::utils::serialization;

QString name_for_ansi_color(ANSIColor color) {
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

    invariant_violation("Invalid ANSIColor value (%lld)", (int64_t)color);
}

QString symbol_for_ansi_color(ANSIColor color) {
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

    invariant_violation("Invalid ANSIColor value (%lld)", (int64_t)color);
}

ANSIColor ansi_color_from_symbol(IMM(QString) symbol) {
    const static QMap<QString, ANSIColor> MAP = {
        { "black",    ANSIColor::BLACK },
        { "red",      ANSIColor::RED },
        { "green",    ANSIColor::GREEN },
        { "yellow",   ANSIColor::YELLOW },
        { "blue",     ANSIColor::BLUE },
        { "magenta",  ANSIColor::MAGENTA },
        { "cyan",     ANSIColor::CYAN },
        { "white",    ANSIColor::WHITE },
        { "extended", ANSIColor::EXTENDED },
    };

    if (MAP.contains(symbol)) {
        return MAP[symbol];
    }

    invariant_violation("Invalid ANSIColor symbol ('%s')", QP(symbol));
}

QDebug operator<< (QDebug stream, ANSIColor color) {
    stream << QP(name_for_ansi_color(color));

    return stream;
}

QDataStream& operator>> (QDataStream& mut_stream, ANSIColor& mut_color) {
    uint8_t code = must_deserialize(mut_stream, uint8_t);

    switch ((ANSIColor)code) {
        case ANSIColor::BLACK:
        case ANSIColor::RED:
        case ANSIColor::GREEN:
        case ANSIColor::YELLOW:
        case ANSIColor::BLUE:
        case ANSIColor::MAGENTA:
        case ANSIColor::CYAN:
        case ANSIColor::WHITE:
        case ANSIColor::EXTENDED:
            mut_color = (ANSIColor)code;
            break;
        default:
            invariant_violation("Invalid deserialized ANSIColor value (%lld)", (int64_t)code);
    }

    return mut_stream;
}

QDataStream& operator<< (QDataStream& mut_stream, ANSIColor color) {
    mut_stream << (uint8_t)color;

    return mut_stream;
}

}}
