/**
 * intermediate_format/events/system/RawWindowClosedEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 6dc76d223b7868f2492686358bd1cab5b626dc3cf4140ff7284ba45f205478fa
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWWINDOWCLOSEDEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWWINDOWCLOSEDEVENT_H

#include "intermediate_format/events/RawEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

struct RawWindowClosedEvent : RawEvent {
public:
    RawWindowClosedEvent(IMM(ApparentTime) timestamp, uint index);

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWWINDOWCLOSEDEVENT_H
