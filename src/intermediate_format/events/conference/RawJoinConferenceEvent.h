/**
 * intermediate_format/events/conference/RawJoinConferenceEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): ca9b477a70800bb053203f457dd170faab2e3c4786b622e19a654938afb4d62e
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWJOINCONFERENCEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWJOINCONFERENCEEVENT_H

#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::subjects;

struct RawJoinConferenceEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> subject;
    optional<RawMessageContent> message;

    RawJoinConferenceEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) subject);

    virtual RawEventSubType subType() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWJOINCONFERENCEEVENT_H
