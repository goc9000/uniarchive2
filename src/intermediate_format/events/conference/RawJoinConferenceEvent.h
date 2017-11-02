/**
 * intermediate_format/events/conference/RawJoinConferenceEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 812e7cdd541ad4beb3e1f96a553dfea4fa1622234b012b67083e061b07278339
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

    RawJoinConferenceEvent(IMM(ApparentTime) timestamp, uint32_t index, TAKE(ApparentSubject) subject);

    virtual RawEventSubType subType() const;

    static CEDE(RawJoinConferenceEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    void visitSubjectsImpl(ApparentSubjectVisitor& visitor);

    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWJOINCONFERENCEEVENT_H
