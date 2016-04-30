/**
 * intermediate_format/events/IFAnswerCallEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_IFANSWERCALLEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_IFANSWERCALLEVENT_H

#include <memory>

#include <QtDebug>

#include "intermediate_format/events/IntermediateFormatEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/language/shortcuts.h"

using namespace std;
using namespace uniarchive2::intermediate_format::subjects;

namespace uniarchive2 { namespace intermediate_format { namespace events {

struct IFAnswerCallEvent : IntermediateFormatEvent {
public:
    unique_ptr<ApparentSubject> receiver;
    unique_ptr<ApparentSubject> caller;

    IFAnswerCallEvent(
        IMM(ApparentTime) timestamp,
        unsigned int index,
        unique_ptr<ApparentSubject> receiver
    );

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_IFANSWERCALLEVENT_H
