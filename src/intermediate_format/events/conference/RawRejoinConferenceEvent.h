/**
 * intermediate_format/events/conference/RawRejoinConferenceEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 997a75c99de8545a82b295db927f3b728c739c685ecb351433ddcd443933f168
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWREJOINCONFERENCEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWREJOINCONFERENCEEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawRejoinConferenceEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> subject;
    vector<unique_ptr<ApparentSubject>> peers;

    RawRejoinConferenceEvent(
        IMM(ApparentTime) timestamp,
        uint32_t index,
        TAKE(ApparentSubject) subject,
        TAKE_VEC(ApparentSubject) peers
    );

    virtual RawEventSubType subType() const;

    static CEDE(RawRejoinConferenceEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWREJOINCONFERENCEEVENT_H
