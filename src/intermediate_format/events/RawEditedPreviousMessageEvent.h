/**
 * intermediate_format/events/RawEditedPreviousMessageEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 4496e7f2a37742869b3cd43f9482fa96f20ae4aa02a44a7f6fdd6d724916dcac
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEDITEDPREVIOUSMESSAGEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEDITEDPREVIOUSMESSAGEEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawEditedPreviousMessageEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> subject;

    RawEditedPreviousMessageEvent(IMM(ApparentTime) timestamp, uint32_t index, TAKE(ApparentSubject) subject);

    virtual RawEventSubType subType() const;

    static CEDE(RawEditedPreviousMessageEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEDITEDPREVIOUSMESSAGEEVENT_H
