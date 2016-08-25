/**
 * intermediate_format/events/conference/RawRemoveFromConferenceEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWREMOVEFROMCONFERENCEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWREMOVEFROMCONFERENCEEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawRemoveFromConferenceEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> remover;
    unique_ptr<ApparentSubject> removee;

    RawRemoveFromConferenceEvent(
        IMM(ApparentTime) timestamp,
        uint index,
        TAKE(ApparentSubject) remover,
        TAKE(ApparentSubject) removee
    );
    RawRemoveFromConferenceEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) removee);

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWREMOVEFROMCONFERENCEEVENT_H
