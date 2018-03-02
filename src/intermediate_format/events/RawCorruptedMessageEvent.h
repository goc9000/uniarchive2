/**
 * intermediate_format/events/RawCorruptedMessageEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 5df62946bda892622ad110a2a85c3a57b2544a81b9c5a5f9dcccfe33cbeb7b69
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCORRUPTEDMESSAGEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCORRUPTEDMESSAGEEVENT_H

#include "intermediate_format/events/RawEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

struct RawCorruptedMessageEvent : RawEvent {
public:
    QByteArray rawData;

    RawCorruptedMessageEvent(IMM(ApparentTime) timestamp, uint32_t index, IMM(QByteArray) raw_data);

    virtual RawEventSubType subType() const;

    static CEDE(RawCorruptedMessageEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IMM(visit_subjects_callback_t) callback) const;
    bool alterSubjectsImpl(IMM(alter_subjects_callback_t) callback);

    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCORRUPTEDMESSAGEEVENT_H
