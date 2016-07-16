/**
 * intermediate_format/events/conference/RawRejoinConferenceEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWREJOINCONFERENCEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWREJOINCONFERENCEEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/language/shortcuts.h"

#include <QtDebug>

#include <vector>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace std;
using namespace uniarchive2::intermediate_format::subjects;

struct RawRejoinConferenceEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> subject;
    vector<unique_ptr<ApparentSubject>> peers;

    RawRejoinConferenceEvent(
        IMM(ApparentTime) timestamp,
        unsigned int index,
        TAKE(ApparentSubject) subject,
        TAKE_VEC(ApparentSubject) peers
    );
    RawRejoinConferenceEvent(
        IMM(ApparentTime) timestamp,
        unsigned int index,
        TAKE(ApparentSubject) subject
    );

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWREJOINCONFERENCEEVENT_H
