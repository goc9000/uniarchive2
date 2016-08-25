/**
 * intermediate_format/events/conference/RawCreateConferenceEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCREATECONFERENCEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCREATECONFERENCEEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawCreateConferenceEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> creator;
    QString conferenceName;

    RawCreateConferenceEvent(
        IMM(ApparentTime) timestamp,
        uint index,
        TAKE(ApparentSubject) creator,
        IMM(QString) conferenceName = ""
    );

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCREATECONFERENCEEVENT_H
