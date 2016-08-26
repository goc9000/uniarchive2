/**
 * intermediate_format/events/conference/RawSetSkypeChatRoleEvent.h
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
        uint index,
        TAKE(ApparentSubject) setter,
        TAKE(ApparentSubject) target,
        SkypeChatRole role
    );
    RawSetSkypeChatRoleEvent(
        IMM(ApparentTime) timestamp,
        uint index,
        TAKE(ApparentSubject) target,
        SkypeChatRole role
    );

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSETSKYPECHATROLEEVENT_H
