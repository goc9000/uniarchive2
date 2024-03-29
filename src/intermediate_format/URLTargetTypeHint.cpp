/**
 * intermediate_format/URLTargetTypeHint.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 32ac329f1e438236a2c20304d0696279b78975416b1d283024fd9c5892d907e7
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/URLTargetTypeHint.h"
#include "utils/qt/shortcuts.h"
#include "utils/serialization/deserialization_helpers.h"

#include <QMap>
#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format {

using namespace uniarchive2::utils::serialization;

QString name_for_url_target_type_hint(URLTargetTypeHint hint) {
    switch (hint) {
        case URLTargetTypeHint::VIDEO:
            return "video";
        case URLTargetTypeHint::TWEET:
            return "tweet";
    }

    invariant_violation("Invalid URLTargetTypeHint value (%lld)", (int64_t)hint);
}

QString symbol_for_url_target_type_hint(URLTargetTypeHint hint) {
    switch (hint) {
        case URLTargetTypeHint::VIDEO:
            return "video";
        case URLTargetTypeHint::TWEET:
            return "tweet";
    }

    invariant_violation("Invalid URLTargetTypeHint value (%lld)", (int64_t)hint);
}

URLTargetTypeHint url_target_type_hint_from_symbol(IMM(QString) symbol) {
    const static QMap<QString, URLTargetTypeHint> MAP = {
        { "video", URLTargetTypeHint::VIDEO },
        { "tweet", URLTargetTypeHint::TWEET },
    };

    if (MAP.contains(symbol)) {
        return MAP[symbol];
    }

    invariant_violation("Invalid URLTargetTypeHint symbol ('%s')", QP(symbol));
}

QDebug operator<< (QDebug stream, URLTargetTypeHint hint) {
    stream << QP(name_for_url_target_type_hint(hint));

    return stream;
}

QDataStream& operator>> (QDataStream& mut_stream, URLTargetTypeHint& mut_hint) {
    uint8_t code = must_deserialize(mut_stream, uint8_t);

    switch ((URLTargetTypeHint)code) {
        case URLTargetTypeHint::VIDEO:
        case URLTargetTypeHint::TWEET:
            mut_hint = (URLTargetTypeHint)code;
            break;
        default:
            invariant_violation("Invalid deserialized URLTargetTypeHint value (%lld)", (int64_t)code);
    }

    return mut_stream;
}

QDataStream& operator<< (QDataStream& mut_stream, URLTargetTypeHint hint) {
    mut_stream << (uint8_t)hint;

    return mut_stream;
}

}}
