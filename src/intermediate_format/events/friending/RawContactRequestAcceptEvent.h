/**
 * intermediate_format/events/friending/RawContactRequestAcceptEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCONTACTREQUESTACCEPTEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCONTACTREQUESTACCEPTEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawContactRequestAcceptEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> acceptor;
    unique_ptr<ApparentSubject> acceptee;

    RawContactRequestAcceptEvent(
        IMM(ApparentTime) timestamp,
        uint index,
        TAKE(ApparentSubject) acceptor,
        TAKE(ApparentSubject) acceptee
    );

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCONTACTREQUESTACCEPTEVENT_H
