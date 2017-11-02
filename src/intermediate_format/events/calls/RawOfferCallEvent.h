/**
 * intermediate_format/events/calls/RawOfferCallEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 6220425bf266b642471e469f33b54404318ad6e5490c8caa3540efe9b966feb7
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWOFFERCALLEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWOFFERCALLEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawOfferCallEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> caller;

    RawOfferCallEvent(IMM(ApparentTime) timestamp, uint32_t index, TAKE(ApparentSubject) caller);

    virtual RawEventSubType subType() const;

    static CEDE(RawOfferCallEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    void visitSubjectsImpl(ApparentSubjectVisitor& visitor);

    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWOFFERCALLEVENT_H
