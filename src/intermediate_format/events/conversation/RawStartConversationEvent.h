/**
 * intermediate_format/events/conversation/RawStartConversationEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTARTCONVERSATIONEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTARTCONVERSATIONEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawStartConversationEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> initiator;
    unique_ptr<ApparentSubject> peer;

    RawStartConversationEvent(IMM(ApparentTime) timestamp, uint index);
    RawStartConversationEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) initiator);
    RawStartConversationEvent(
        IMM(ApparentTime) timestamp,
        uint index,
        TAKE(ApparentSubject) initiator,
        TAKE(ApparentSubject) peer
    );

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTARTCONVERSATIONEVENT_H
