/**
 * intermediate_format/events/RawSendContactsAcceptEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSENDCONTACTSEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSENDCONTACTSEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/language/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace std;
using namespace uniarchive2::intermediate_format::subjects;

struct RawSendContactsEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> sender;
    vector<unique_ptr<ApparentSubject>> contacts;

    RawSendContactsEvent(
        IMM(ApparentTime) timestamp,
        uint index,
        TAKE(ApparentSubject) sender,
        TAKE_VEC(ApparentSubject) contacts
    );

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSENDCONTACTSEVENT_H
