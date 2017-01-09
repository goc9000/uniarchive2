/**
 * intermediate_format/events/conference/RawChangeTopicEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 67022f58ed720edbb71ff51073f14ba799af89175885d9ca94febc79a8ae8b09
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCHANGETOPICEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCHANGETOPICEVENT_H

#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::subjects;

struct RawChangeTopicEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> subject;
    RawMessageContent topic;

    RawChangeTopicEvent(
        IMM(ApparentTime) timestamp,
        uint32_t index,
        TAKE(ApparentSubject) subject,
        RawMessageContent&& topic
    );

    virtual RawEventSubType subType() const;

    static CEDE(RawChangeTopicEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCHANGETOPICEVENT_H
