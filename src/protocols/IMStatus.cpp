/**
 * protocols/IMStatus.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 374e35e11920df801c2118ec7e041d67d4b1f38323815edbe2bc699b75a3788d
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "protocols/IMStatus.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace protocols {

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

QDebug operator<< (QDebug stream, IMStatus status) {
    stream << QP(name_for_im_status(status));

    return stream;
}

QDataStream& operator<< (QDataStream& mut_stream, IMStatus status) {
    mut_stream << (uint8_t)status;

    return mut_stream;
}

}}
