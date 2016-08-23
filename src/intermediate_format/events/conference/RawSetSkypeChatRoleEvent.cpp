/**
 * intermediate_format/events/conference/RawSetSkypeChatRoleEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawSetSkypeChatRoleEvent.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace protocols::skype;

RawSetSkypeChatRoleEvent::RawSetSkypeChatRoleEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) setter,
    TAKE(ApparentSubject) target,
    SkypeChatRole role
): RawEvent(timestamp, index), setter(move(setter)), target(move(target)), role(role) {
}

RawSetSkypeChatRoleEvent::RawSetSkypeChatRoleEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) target,
    SkypeChatRole role
): RawEvent(timestamp, index), target(move(target)), role(role) {
}

QString RawSetSkypeChatRoleEvent::eventName() const {
    return "SetSkypeChatRole";
}

void RawSetSkypeChatRoleEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (setter) {
        stream << " setter=" << setter.get();
    }
    stream << " target=" << target.get() << " role=" << role;
}

}}}
