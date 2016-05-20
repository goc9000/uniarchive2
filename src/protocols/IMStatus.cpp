/**
 * protocols/IMStatus.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QtDebug>

#include "protocols/IMStatus.h"
#include "utils/qt/shortcuts.h"

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
}

QDebug operator<< (QDebug stream, IMStatus status) {
    stream << QP(name_for_im_status(status));

    return stream;
}

}}
