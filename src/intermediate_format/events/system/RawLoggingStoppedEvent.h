/**
 * intermediate_format/events/system/RawLoggingStoppedEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWLOGGINGSTOPPEDEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWLOGGINGSTOPPEDEVENT_H

#include "intermediate_format/events/RawEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

struct RawLoggingStoppedEvent : RawEvent {
public:
    RawLoggingStoppedEvent(IMM(ApparentTime) timestamp, uint index);

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWLOGGINGSTOPPEDEVENT_H
