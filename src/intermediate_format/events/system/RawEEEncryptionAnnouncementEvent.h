/**
 * intermediate_format/events/system/RawEEEncryptionAnnouncementEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): e49122671d654259c2f38ba2dba21ea6ed417d19de3f6ddf3fb507f83c8dd01d
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
    void visitSubjectsImpl(ApparentSubjectVisitor& visitor);

    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEEENCRYPTIONANNOUNCEMENTEVENT_H
