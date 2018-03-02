/**
 * intermediate_format/events/friending/RawContactRequestAcceptEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): c5d7b9cfab6c6fec9d73dc3b6745b8f8503524d4b208bd0181eda552ee314b54
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCONTACTREQUESTACCEPTEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCONTACTREQUESTACCEPTEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;

struct RawContactRequestAcceptEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> acceptor;
    unique_ptr<ApparentSubject> acceptee;

    RawContactRequestAcceptEvent(
        IMM(ApparentTime) timestamp,
        uint32_t index,
        TAKE(ApparentSubject) acceptor,
        TAKE(ApparentSubject) acceptee
    );

    virtual RawEventSubType subType() const;

    static CEDE(RawContactRequestAcceptEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IMM(visit_subjects_callback_t) callback) const;
    bool alterSubjectsImpl(IMM(alter_subjects_callback_t) callback);

    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCONTACTREQUESTACCEPTEVENT_H
