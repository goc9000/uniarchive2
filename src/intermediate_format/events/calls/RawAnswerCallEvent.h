/**
 * intermediate_format/events/calls/RawAnswerCallEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): f2598bb7300570a226c80da8e5c9cf195ac4a7918d0e60a62cafbb6e1630dab4
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWANSWERCALLEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWANSWERCALLEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawAnswerCallEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> receiver;
    unique_ptr<ApparentSubject> caller;

    RawAnswerCallEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) receiver);

    virtual RawEventSubType subType() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWANSWERCALLEVENT_H
