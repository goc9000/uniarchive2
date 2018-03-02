/**
 * intermediate_format/events/conference/RawLeaveConferenceEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 5909aa04df4fa59690a7a0d9aea6dd57eea51f943e7000c6fe3e701014c6bc8e
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWLEAVECONFERENCEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWLEAVECONFERENCEEVENT_H

#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::subjects;

struct RawLeaveConferenceEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> subject;
    optional<RawMessageContent> message;

    RawLeaveConferenceEvent(IMM(ApparentTime) timestamp, uint32_t index, TAKE(ApparentSubject) subject);

    virtual RawEventSubType subType() const;

    static CEDE(RawLeaveConferenceEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IMM(visit_subjects_callback_t) callback) const;
    bool alterSubjectsImpl(IMM(alter_subjects_callback_t) callback);

    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWLEAVECONFERENCEEVENT_H
