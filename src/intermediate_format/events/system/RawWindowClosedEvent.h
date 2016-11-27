/**
 * intermediate_format/events/system/RawWindowClosedEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 5244ea24c60770d9595e0433e4d63067f4454dc4502d314fa164255875513854
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
    RawWindowClosedEvent(IMM(ApparentTime) timestamp, uint32_t index);

    virtual RawEventSubType subType() const;

protected:
    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWWINDOWCLOSEDEVENT_H
