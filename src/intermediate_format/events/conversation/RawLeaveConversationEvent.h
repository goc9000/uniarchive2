/**
 * intermediate_format/events/conversation/RawLeaveConversationEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 72bb6143aaf4b7c22a6c0a5043d51978ea179089d1dc53a8da61fc6f7f9f0cbb
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWLEAVECONVERSATIONEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWLEAVECONVERSATIONEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawLeaveConversationEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> subject;

    RawLeaveConversationEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) subject);

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWLEAVECONVERSATIONEVENT_H
