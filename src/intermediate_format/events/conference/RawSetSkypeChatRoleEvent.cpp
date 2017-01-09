/**
 * intermediate_format/events/conference/RawSetSkypeChatRoleEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): bbc04de45468115d1ee41882d11002b1317986fb6e524d11c930989ce8b493af
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawSetSkypeChatRoleEvent.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace uniarchive2::utils::serialization;

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

CEDE(RawSetSkypeChatRoleEvent) RawSetSkypeChatRoleEvent::deserializeFromStream(
    QDataStream& mut_stream,
    bool skip_type
) {
    invariant_violation("RawSetSkypeChatRoleEvent deserialization not yet implemented");
}

void RawSetSkypeChatRoleEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    serialize_optional_unique_ptr(mut_stream, setter);
    mut_stream << target << role;
}

void RawSetSkypeChatRoleEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (setter) {
        stream << " setter=" << setter;
    }
    stream << " target=" << target << " role=" << role;
}

}}}
