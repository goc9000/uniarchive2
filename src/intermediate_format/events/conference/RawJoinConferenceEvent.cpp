/**
 * intermediate_format/events/conference/RawJoinConferenceEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): e4964ddd61f2d61530c6c7345519b76e4914f46980b5db7cd20c4e01df716ade
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawJoinConferenceEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawJoinConferenceEvent::RawJoinConferenceEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) subject)
  : RawEvent(timestamp, index), subject(move(subject)) {
    sanityCheckMandatoryParameters();
}

void RawJoinConferenceEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
}

RawEventSubType RawJoinConferenceEvent::subType() const {
    return RawEventSubType::JOIN_CONFERENCE;
}

void RawJoinConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
    if (message) {
        stream << " message=" << *message;
    }
}

}}}
