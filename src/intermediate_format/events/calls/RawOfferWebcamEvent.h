/**
 * intermediate_format/events/calls/RawOfferWebcamEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 2f04246318250e270fc2dd095f1a0d7197f10ee3c5f1b892267e3c0fab869f5d
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

    RawOfferWebcamEvent(IMM(ApparentTime) timestamp, uint32_t index, TAKE(ApparentSubject) sender);

    virtual RawEventSubType subType() const;

    static CEDE(RawOfferWebcamEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IMM(visit_subjects_callback_t) callback) const;
    bool alterSubjectsImpl(IMM(alter_subjects_callback_t) callback);

    virtual void serializeFailableEventDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeFailableEventDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWOFFERWEBCAMEVENT_H
