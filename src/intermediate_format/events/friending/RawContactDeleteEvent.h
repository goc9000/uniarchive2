/**
 * intermediate_format/events/friending/RawContactDeleteEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 679a3d6283ac0675440bd88ec5a88d6aa8a0aff76b175cc98c2a69206bcab503
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCONTACTDELETEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCONTACTDELETEEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawContactDeleteEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> deleter;
    unique_ptr<ApparentSubject> deletee;

    RawContactDeleteEvent(
        IMM(ApparentTime) timestamp,
        uint32_t index,
        TAKE(ApparentSubject) deleter,
        TAKE(ApparentSubject) deletee
    );

    virtual RawEventSubType subType() const;

    static CEDE(RawContactDeleteEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    void visitSubjectsImpl(ApparentSubjectVisitor& visitor);

    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCONTACTDELETEEVENT_H
