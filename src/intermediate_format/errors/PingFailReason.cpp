/**
 * intermediate_format/errors/PingFailReason.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): eb6a2a00f3c98bad89dc977f73e3efcbe6e2f11fbfa63d36860a9044e5cc0f4c
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/errors/PingFailReason.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace errors {

QString name_for_ping_fail_reason(PingFailReason reason) {
    switch (reason) {
        case PingFailReason::UNDETERMINED:
            return "undetermined";
        case PingFailReason::BLOCKED_OR_UNSUPPORTED:
            return "blocked or unsupported";
    }

    invariant_violation("Invalid PingFailReason value (%d)", reason);
}

QDebug operator<< (QDebug stream, PingFailReason reason) {
    stream << QP(name_for_ping_fail_reason(reason));

    return stream;
}

}}}
