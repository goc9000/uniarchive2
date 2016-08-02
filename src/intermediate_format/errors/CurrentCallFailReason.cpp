/**
 * intermediate_format/errors/CurrentCallFailReason.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/errors/CurrentCallFailReason.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace errors {

QString name_for_current_call_fail_reason(CurrentCallFailReason reason) {
    switch (reason) {
        case CurrentCallFailReason::UNDETERMINED:
            return "undetermined";
        case CurrentCallFailReason::CONNECTION_DROPPED:
            return "connection dropped";
    }

    invariant_violation("Invalid CurrentCallFailReason (value: %d)", reason);
}

QDebug operator<< (QDebug stream, CurrentCallFailReason reason) {
    stream << QP(name_for_current_call_fail_reason(reason));

    return stream;
}

}}}