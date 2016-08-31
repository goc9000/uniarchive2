/**
 * intermediate_format/events/status/RawDisconnectedEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): a9f012a9788cf16aab817435fb3720f5dd59f3ba662162c2f3172869244ce734
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWDISCONNECTEDEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWDISCONNECTEDEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawDisconnectedEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> subject;

    RawDisconnectedEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) subject);

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWDISCONNECTEDEVENT_H
