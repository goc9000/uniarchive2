/**
 * intermediate_format/events/IFLeaveConferenceEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_IFLEAVECONFERENCEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_IFLEAVECONFERENCEEVENT_H

#include <memory>

#include <QtDebug>

#include "utils/external_libs/optional.hpp"

#include "intermediate_format/content/IntermediateFormatMessageContent.h"
#include "intermediate_format/events/IntermediateFormatEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

using namespace std;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::subjects;

namespace uniarchive2 { namespace intermediate_format { namespace events {

struct IFLeaveConferenceEvent : IntermediateFormatEvent {
public:
    shared_ptr<ApparentSubject> subject;
    optional<IntermediateFormatMessageContent> message;

    IFLeaveConferenceEvent(ApparentTime timestamp, unsigned int index, shared_ptr<ApparentSubject> subject);

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_IFLEAVECONFERENCEEVENT_H
