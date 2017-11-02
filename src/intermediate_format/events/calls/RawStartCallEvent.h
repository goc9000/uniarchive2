/**
 * intermediate_format/events/calls/RawStartCallEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 3ce2b2af8f062bbb4e773d10926d049c5a817dbc69727971ba1a88ba20b6c57d
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTARTCALLEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTARTCALLEVENT_H

#include "intermediate_format/errors/StartCallFailReason.h"
#include "intermediate_format/events/RawFailableEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::errors;
using namespace uniarchive2::intermediate_format::subjects;

struct RawStartCallEvent : RawFailableEvent<StartCallFailReason> {
public:
    unique_ptr<ApparentSubject> initiator;
    vector<unique_ptr<ApparentSubject>> peers;

    optional<uint16_t> durationSeconds;

    optional<QString> skypeCallGUID;
    optional<QString> syntheticCallGUID;
    optional<uint64_t> correspondingSkypeCallID;

    RawStartCallEvent(
        IMM(ApparentTime) timestamp,
        uint32_t index,
        TAKE(ApparentSubject) initiator,
        TAKE_VEC(ApparentSubject) peers
    );
    RawStartCallEvent(
        IMM(ApparentTime) timestamp,
        uint32_t index,
        TAKE(ApparentSubject) initiator,
        TAKE(ApparentSubject) peer
    );

    virtual RawEventSubType subType() const;

    static CEDE(RawStartCallEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    void visitSubjectsImpl(ApparentSubjectVisitor& visitor);

    virtual void serializeFailableEventDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeFailableEventDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTARTCALLEVENT_H
