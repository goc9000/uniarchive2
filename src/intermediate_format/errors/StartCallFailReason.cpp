/**
 * intermediate_format/errors/StartCallFailReason.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/errors/StartCallFailReason.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace errors {

QString name_for_start_call_fail_reason(StartCallFailReason reason) {
    switch (reason) {
        case StartCallFailReason::UNDETERMINED:
            return "undetermined";
        case StartCallFailReason::NO_ANSWER:
            return "no answer";
        case StartCallFailReason::BUSY:
            return "peer busy";
        case StartCallFailReason::PRIVACY_BLOCKED:
            return "blocked by privacy settings";
        case StartCallFailReason::CALL_REJECTED:
            return "call rejected";
    }

    invariant_violation("Invalid StartCallFailReason (value: %d)", reason);
}

QDebug operator<< (QDebug stream, StartCallFailReason reason) {
    stream << QP(name_for_start_call_fail_reason(reason));

    return stream;
}

}}}