/**
 * intermediate_format/events/RawSendContactsEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 8d750e70969d376ff469e5ad8e3e30fde01151c790b20dc5bec167a0357279e2
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

namespace uniarchive2 { namespace intermediate_format { namespace events {

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

    virtual RawEventSubType subType() const;

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSENDCONTACTSEVENT_H
