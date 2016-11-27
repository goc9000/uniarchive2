/**
 * intermediate_format/events/calls/RawEndCallEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 9dcaf542df814c7cdc25e38e462f4722824828ebfe7a7c3dad01123c97734fa7
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/calls/RawEndCallEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawEndCallEvent::RawEndCallEvent(IMM(ApparentTime) timestamp, uint32_t index) : RawEvent(timestamp, index) {
}

RawEventSubType RawEndCallEvent::subType() const {
    return RawEventSubType::END_CALL;
}

void RawEndCallEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    // TODO: add field serialization code
}

void RawEndCallEvent::writeDetailsToDebugStream(QDebug stream) const {
    if (ender) {
        stream << " ender=" << ender.get();
    }
    if (reasonCallFailed) {
        stream << " reason_call_failed=" << *reasonCallFailed;
    }
    if (skypeCallGUID) {
        stream << " skype_call_guid=" << *skypeCallGUID;
    }
    if (syntheticCallGUID) {
        stream << " synthetic_call_guid=" << *syntheticCallGUID;
    }
    if (correspondingSkypeCallID) {
        stream << " corresponding_skype_call_id=" << *correspondingSkypeCallID;
    }
}

}}}
