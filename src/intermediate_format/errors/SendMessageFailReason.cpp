/**
 * intermediate_format/errors/SendMessageFailReason.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/errors/SendMessageFailReason.h"
#include "utils/qt/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace errors {

QString name_for_send_message_fail_reason(SendMessageFailReason reason) {
    switch (reason) {
        case SendMessageFailReason::UNDETERMINED:
            return "undetermined";
        case SendMessageFailReason::MESSAGE_TOO_LARGE:
            return "message too large";
        case SendMessageFailReason::RECIPIENT_OFFLINE:
            return "recipient offline";
        case SendMessageFailReason::CONNECTION_ERROR:
            return "connection error";
    }

    invariant_violation("Invalid SendMessageFailReason (value: %d)", reason);
}

QDebug operator<< (QDebug stream, SendMessageFailReason reason) {
    stream << QP(name_for_send_message_fail_reason(reason));

    return stream;
}

}}}
