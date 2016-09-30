/**
 * intermediate_format/events/RawPingEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 546f13e71b38038a2444007150fdd1e9c5cc2796f6a0010c4a88d0a8a3eedd9b
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWPINGEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWPINGEVENT_H

#include "intermediate_format/errors/PingFailReason.h"
#include "intermediate_format/events/RawFailableEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::errors;
using namespace uniarchive2::intermediate_format::subjects;

struct RawPingEvent : RawFailableEvent<PingFailReason> {
public:
    unique_ptr<ApparentSubject> pinger;
    unique_ptr<ApparentSubject> pingee;

    RawPingEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) pinger);

    virtual RawEventSubType subType() const;

protected:
    virtual void writeFailableEventDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWPINGEVENT_H
