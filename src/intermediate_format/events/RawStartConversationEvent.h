/**
 * intermediate_format/events/RawStartConversationEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_IFSTARTCONVERSATIONEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_IFSTARTCONVERSATIONEVENT_H

#include <memory>

#include <QtDebug>

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/language/shortcuts.h"

using namespace std;
using namespace uniarchive2::intermediate_format::subjects;

namespace uniarchive2 { namespace intermediate_format { namespace events {

struct RawStartConversationEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> initiator;

    RawStartConversationEvent(IMM(ApparentTime) timestamp, unsigned int index);
    RawStartConversationEvent(IMM(ApparentTime) timestamp, unsigned int index, TAKE(ApparentSubject) initiator);

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_IFSTARTCONVERSATIONEVENT_H