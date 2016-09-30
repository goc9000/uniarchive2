/**
 * intermediate_format/events/friending/RawContactRequestEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 39a147bc18b3c04b02c403524a22c6828a75dcc19f6d3c088d710a17e850390e
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCONTACTREQUESTEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCONTACTREQUESTEVENT_H

#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::subjects;

struct RawContactRequestEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> sender;
    unique_ptr<ApparentSubject> receiver;
    RawMessageContent message;

    RawContactRequestEvent(
        IMM(ApparentTime) timestamp,
        uint index,
        TAKE(ApparentSubject) sender,
        TAKE(ApparentSubject) receiver,
        RawMessageContent&& message
    );

    virtual RawEventSubType subType() const;

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCONTACTREQUESTEVENT_H
