/**
 * intermediate_format/events/conference/RawSetSkypeChatRoleEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): d488af30aa278f25ece6e5c93c07b747bd4d150e84d95f3d580e8ce551325b68
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawSetSkypeChatRoleEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawSetSkypeChatRoleEvent::RawSetSkypeChatRoleEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) target,
    SkypeChatRole role
) : RawEvent(timestamp, index), target(move(target)), role(role) {
    sanityCheckMandatoryParameters();
}

RawSetSkypeChatRoleEvent::RawSetSkypeChatRoleEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) setter,
    TAKE(ApparentSubject) target,
    SkypeChatRole role
) : RawEvent(timestamp, index), setter(move(setter)), target(move(target)), role(role) {
    sanityCheckMandatoryParameters();
    invariant(this->setter, "Parameter 'setter' cannot have empty value");
}

void RawSetSkypeChatRoleEvent::sanityCheckMandatoryParameters() const {
    invariant(target, "Parameter 'target' cannot have empty value");
}

RawEventSubType RawSetSkypeChatRoleEvent::subType() const {
    return RawEventSubType::SET_SKYPE_CHAT_ROLE;
}

void RawSetSkypeChatRoleEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    // TODO: add field serialization code
}

void RawSetSkypeChatRoleEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (setter) {
        stream << " setter=" << setter.get();
    }
    stream << " target=" << target.get() << " role=" << role;
}

}}}
