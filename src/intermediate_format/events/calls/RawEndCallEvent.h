/**
 * intermediate_format/events/calls/RawEndCallEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): e7ed5bdc8bfe7efd6b9b96bd468d9aaf0168eb4f2402dd4685095a58cbdec972
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWENDCALLEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWENDCALLEVENT_H

#include "intermediate_format/errors/CurrentCallFailReason.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::errors;
using namespace uniarchive2::intermediate_format::subjects;

struct RawEndCallEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> ender;

    optional<CurrentCallFailReason> reasonCallFailed;

    optional<QString> skypeCallGUID;
    optional<QString> syntheticCallGUID;
    optional<uint64_t> correspondingSkypeCallID;

    RawEndCallEvent(IMM(ApparentTime) timestamp, uint32_t index);

    virtual RawEventSubType subType() const;

    static CEDE(RawEndCallEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IApparentSubjectVisitor& visitor);

    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWENDCALLEVENT_H
