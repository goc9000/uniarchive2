/**
 * intermediate_format/events/conference/RawCreateConferenceEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): a304427fa2fc7acf74d1df05d5db4956a90c30e2764827cabb51084bfe606cb7
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCREATECONFERENCEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCREATECONFERENCEEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawCreateConferenceEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> creator;
    optional<QString> conferenceName;

    RawCreateConferenceEvent(IMM(ApparentTime) timestamp, uint32_t index, TAKE(ApparentSubject) creator);
    RawCreateConferenceEvent(
        IMM(ApparentTime) timestamp,
        uint32_t index,
        TAKE(ApparentSubject) creator,
        IMM(QString) conference_name
    );

    virtual RawEventSubType subType() const;

    static CEDE(RawCreateConferenceEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCREATECONFERENCEEVENT_H
