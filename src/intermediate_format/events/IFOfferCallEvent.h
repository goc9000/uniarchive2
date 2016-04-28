/**
 * intermediate_format/events/IFOfferCallEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_IFOFFERCALLEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_IFOFFERCALLEVENT_H

#include <memory>

#include <QtDebug>

#include "intermediate_format/events/IntermediateFormatEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

using namespace std;
using namespace uniarchive2::intermediate_format::subjects;

namespace uniarchive2 { namespace intermediate_format { namespace events {

struct IFOfferCallEvent : IntermediateFormatEvent {
public:
    unique_ptr<ApparentSubject> caller;

    IFOfferCallEvent(
        const ApparentTime& timestamp,
        unsigned int index,
        unique_ptr<ApparentSubject> caller
    );

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_IFOFFERCALLEVENT_H
