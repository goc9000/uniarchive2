/**
 * intermediate_format/events/conference/RawRejoinConferenceEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): bf37fd1cc105f2bc11e1d8759688a1242407c57d4b3505de83dddea528b4c3b4
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawRejoinConferenceEvent.h"
#include "utils/qt/debug_extras.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawRejoinConferenceEvent::RawRejoinConferenceEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) subject,
    TAKE_VEC(ApparentSubject) peers
) : RawEvent(timestamp, index), subject(move(subject)), peers(move(peers)) {
    sanityCheckMandatoryParameters();
}

void RawRejoinConferenceEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
    for (IMM(auto) item : peers) {
        invariant(item, "Parameter 'peers' cannot have empty entries");
    }
}

RawEventSubType RawRejoinConferenceEvent::subType() const {
    return RawEventSubType::REJOIN_CONFERENCE;
}

void RawRejoinConferenceEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get() << " peers=" << peers;
}

}}}
