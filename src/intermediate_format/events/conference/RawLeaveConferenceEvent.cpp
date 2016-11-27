/**
 * intermediate_format/events/conference/RawLeaveConferenceEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): a105d8c554674245b3b7348adda87cbf600b55d774b070a2e86f3ffb59a09edb
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawLeaveConferenceEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawLeaveConferenceEvent::RawLeaveConferenceEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) subject
) : RawEvent(timestamp, index), subject(move(subject)) {
    sanityCheckMandatoryParameters();
}

void RawLeaveConferenceEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
}

RawEventSubType RawLeaveConferenceEvent::subType() const {
    return RawEventSubType::LEAVE_CONFERENCE;
}

void RawLeaveConferenceEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    // TODO: add field serialization code
}

void RawLeaveConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
    if (message) {
        stream << " message=" << *message;
    }
}

}}}
