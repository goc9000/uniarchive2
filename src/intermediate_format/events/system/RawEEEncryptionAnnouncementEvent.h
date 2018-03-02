/**
 * intermediate_format/events/system/RawEEEncryptionAnnouncementEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 585b3d1932e58ec0638cd0606869827ced5d3a127018f11d852d2dfefac290b6
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEEENCRYPTIONANNOUNCEMENTEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEEENCRYPTIONANNOUNCEMENTEVENT_H

#include "intermediate_format/events/RawEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

struct RawEEEncryptionAnnouncementEvent : RawEvent {
public:
    RawEEEncryptionAnnouncementEvent(IMM(ApparentTime) timestamp, uint32_t index);

    virtual RawEventSubType subType() const;

    static CEDE(RawEEEncryptionAnnouncementEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IMM(visit_subjects_callback_t) callback);

    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEEENCRYPTIONANNOUNCEMENTEVENT_H
