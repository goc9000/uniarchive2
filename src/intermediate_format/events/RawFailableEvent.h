/**
 * intermediate_format/events/RawFailableEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWFAILABLEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWFAILABLEEVENT_H

#include <memory>

#include <QtDebug>

#include "intermediate_format/events/RawEvent.h"
#include "utils/language/shortcuts.h"

using namespace std;

namespace uniarchive2 { namespace intermediate_format { namespace events {

struct RawFailableEvent : RawEvent {
public:
    enum class FailReason {
        SUCCESSFUL = 0,
        FAILED_UNSUPPORTED,
        FAILED_BLOCKED_OR_UNSUPPORTED
    };
    FailReason reasonFailed = FailReason::SUCCESSFUL;

    RawFailableEvent(IMM(ApparentTime) timestamp, unsigned int index);

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
    virtual void writeFailableEventDetailsToDebugStream(QDebug stream) const = 0;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWFAILABLEEVENT_H
