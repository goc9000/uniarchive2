/**
 * intermediate_format/events/conference/RawSetSkypeChatRoleEvent.h
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 6b35fdca408e4b32b10e793483acb3dbbcffa38216d213f30392f19081249738
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSETSKYPECHATROLEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSETSKYPECHATROLEEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "protocols/skype/SkypeChatRole.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols::skype;

struct RawSetSkypeChatRoleEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> setter;
    unique_ptr<ApparentSubject> target;
    SkypeChatRole role;

    RawSetSkypeChatRoleEvent(
        IMM(ApparentTime) timestamp,
        uint32_t index,
        TAKE(ApparentSubject) target,
        SkypeChatRole role
    );
    RawSetSkypeChatRoleEvent(
        IMM(ApparentTime) timestamp,
        uint32_t index,
        TAKE(ApparentSubject) setter,
        TAKE(ApparentSubject) target,
        SkypeChatRole role
    );

    virtual RawEventSubType subType() const;

    static CEDE(RawSetSkypeChatRoleEvent) deserializeFromStream(QDataStream& mut_stream, bool skip_type=false);

protected:
    bool visitSubjectsImpl(IApparentSubjectVisitor& visitor);

    virtual void serializeDetailsToStream(QDataStream& mut_stream) const;

    virtual void writeDetailsToDebugStream(QDebug stream) const;

private:
    void sanityCheckMandatoryParameters() const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSETSKYPECHATROLEEVENT_H
