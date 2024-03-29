/**
 * protocols/IMStatus.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 3fd8487df6d4dc110ebdadcdc04061c5ed993e3ec56b07b995ea5375980e1f35
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "protocols/IMStatus.h"
#include "utils/qt/shortcuts.h"
#include "utils/serialization/deserialization_helpers.h"

#include <QMap>
#include <QtDebug>

namespace uniarchive2 { namespace protocols {

using namespace uniarchive2::utils::serialization;

QString name_for_im_status(IMStatus status) {
    switch (status) {
        case IMStatus::INVALID:
            return "(invalid)";
        case IMStatus::AVAILABLE:
            return "available";
        case IMStatus::AWAY:
            return "away";
        case IMStatus::IDLE:
            return "idle";
        case IMStatus::OFFLINE:
            return "offline";
    }

    invariant_violation("Invalid IMStatus value (%lld)", (int64_t)status);
}

QString symbol_for_im_status(IMStatus status) {
    switch (status) {
        case IMStatus::INVALID:
            return "invalid";
        case IMStatus::AVAILABLE:
            return "available";
        case IMStatus::AWAY:
            return "away";
        case IMStatus::IDLE:
            return "idle";
        case IMStatus::OFFLINE:
            return "offline";
    }

    invariant_violation("Invalid IMStatus value (%lld)", (int64_t)status);
}

IMStatus im_status_from_symbol(IMM(QString) symbol) {
    const static QMap<QString, IMStatus> MAP = {
        { "invalid",   IMStatus::INVALID },
        { "available", IMStatus::AVAILABLE },
        { "away",      IMStatus::AWAY },
        { "idle",      IMStatus::IDLE },
        { "offline",   IMStatus::OFFLINE },
    };

    if (MAP.contains(symbol)) {
        return MAP[symbol];
    }

    invariant_violation("Invalid IMStatus symbol ('%s')", QP(symbol));
}

QDebug operator<< (QDebug stream, IMStatus status) {
    stream << QP(name_for_im_status(status));

    return stream;
}

QDataStream& operator>> (QDataStream& mut_stream, IMStatus& mut_status) {
    uint8_t code = must_deserialize(mut_stream, uint8_t);

    switch ((IMStatus)code) {
        case IMStatus::INVALID:
        case IMStatus::AVAILABLE:
        case IMStatus::AWAY:
        case IMStatus::IDLE:
        case IMStatus::OFFLINE:
            mut_status = (IMStatus)code;
            break;
        default:
            invariant_violation("Invalid deserialized IMStatus value (%lld)", (int64_t)code);
    }

    return mut_stream;
}

QDataStream& operator<< (QDataStream& mut_stream, IMStatus status) {
    mut_stream << (uint8_t)status;

    return mut_stream;
}

}}
