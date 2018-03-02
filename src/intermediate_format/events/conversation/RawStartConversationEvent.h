/**
 * intermediate_format/events/conversation/RawStartConversationEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): e2196aa75b28b65d6dc23c2e2ff0ecebbe3fd1f29c116d33332f2f9e71b4131c
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTARTCONVERSATIONEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTARTCONVERSATIONEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawStartConversationEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> initiator;
    unique_ptr<ApparentSubject> peer;

    RawStartConversationEvent(IMM(ApparentTime) timestamp, uint32_t index);
    RawStartConversationEvent(IMM(ApparentTime) timestamp, uint32_t index, TAKE(ApparentSubject) initiator);
    RawStartConversationEvent(
        IMM(ApparentTime) timestamp,
        uint32_t index,
        TAKE(ApparentSubject) initiator,
        TAKE(ApparentSubject) peer
    );

    virtual RawEventSubType subType() const;

    static CEDE(RawStartConversationEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IMM(visit_subjects_callback_t) callback) const;
    bool alterSubjectsImpl(IMM(alter_subjects_callback_t) callback);

    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTARTCONVERSATIONEVENT_H
