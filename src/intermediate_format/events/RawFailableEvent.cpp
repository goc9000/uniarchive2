/**
 * intermediate_format/events/RawFailableEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawFailableEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawFailableEvent::RawFailableEvent(IMM(ApparentTime) timestamp, unsigned int index) : RawEvent(timestamp, index) {
};

void RawFailableEvent::writeDetailsToDebugStream(QDebug stream) const {
    writeFailableEventDetailsToDebugStream(stream);

    switch (reasonFailed) {
        case FailReason::SUCCESSFUL:
            break;
        case FailReason::FAILED_UNSUPPORTED:
            stream << " FAILED(UNSUPPORTED)";
            break;
        case FailReason::FAILED_BLOCKED_OR_UNSUPPORTED:
            stream << " FAILED(BLOCKED/UNSUPPORTED)";
            break;
    }
}

}}}
