/**
 * intermediate_format/events/calls/RawOfferWebcamEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWOFFERWEBCAMEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWOFFERWEBCAMEVENT_H

#include "intermediate_format/events/RawFailableEvent.h"
#include "intermediate_format/errors/OfferWebcamFailReason.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/language/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace std;
using namespace uniarchive2::intermediate_format::errors;
using namespace uniarchive2::intermediate_format::subjects;

struct RawOfferWebcamEvent : RawFailableEvent<OfferWebcamFailReason> {
public:
    unique_ptr<ApparentSubject> sender;

    RawOfferWebcamEvent(IMM(ApparentTime) timestamp, unsigned int index, TAKE(ApparentSubject) sender);

    virtual QString eventName() const;

protected:
    virtual void writeFailableEventDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWOFFERWEBCAMEVENT_H
