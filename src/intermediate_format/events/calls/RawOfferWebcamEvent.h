/**
 * intermediate_format/events/calls/RawOfferWebcamEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 4769a2199fa46bf1abe4acdf74bec16da6c34ce73483168a9a23303ab9112696
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWOFFERWEBCAMEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWOFFERWEBCAMEVENT_H

#include "intermediate_format/errors/OfferWebcamFailReason.h"
#include "intermediate_format/events/RawFailableEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::errors;
using namespace uniarchive2::intermediate_format::subjects;

struct RawOfferWebcamEvent : RawFailableEvent<OfferWebcamFailReason> {
public:
    unique_ptr<ApparentSubject> sender;

    RawOfferWebcamEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) sender);

    virtual RawEventSubType subType() const;

protected:
    virtual void writeFailableEventDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWOFFERWEBCAMEVENT_H
