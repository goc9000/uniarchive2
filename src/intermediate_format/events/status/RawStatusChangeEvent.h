/**
 * intermediate_format/events/status/RawStatusChangeEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 8245b29c9e7e419bc3ab4135423e7bddf3eff832a989510d5e62e7bdbfb4e3d4
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTATUSCHANGEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTATUSCHANGEEVENT_H

#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "protocols/IMStatus.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;

struct RawStatusChangeEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> subject;
    IMStatus status;
    optional<RawMessageContent> message;

    RawStatusChangeEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) subject, IMStatus status);

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTATUSCHANGEEVENT_H
