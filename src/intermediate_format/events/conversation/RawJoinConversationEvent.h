/**
 * intermediate_format/events/conversation/RawJoinConversationEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): a7e33e37e2018fd6ccfeb87d42e29529891af28f84346c9a2f1f338b7686afd0
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWJOINCONVERSATIONEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWJOINCONVERSATIONEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawJoinConversationEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> subject;

    RawJoinConversationEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) subject);

    virtual RawEventSubType subType() const;

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWJOINCONVERSATIONEVENT_H
