/**
 * intermediate_format/events/calls/RawStartCallEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): adc07b7e8a7a66aaa9b92c1418d597d1f749efa866b317c7d32954c9c6474f14
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/calls/RawStartCallEvent.h"
#include "utils/qt/debug_extras.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawStartCallEvent::RawStartCallEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) initiator,
    TAKE_VEC(ApparentSubject) peers
) : RawFailableEvent(timestamp, index), initiator(move(initiator)), peers(move(peers)) {
    sanityCheckMandatoryParameters();
}

RawStartCallEvent::RawStartCallEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) initiator,
    TAKE(ApparentSubject) peer
) : RawFailableEvent(timestamp, index), initiator(move(initiator)) {
    peers.push_back(move(peer));
    sanityCheckMandatoryParameters();
}

void RawStartCallEvent::sanityCheckMandatoryParameters() const {
    invariant(initiator, "Parameter 'initiator' cannot have empty value");
}

QString RawStartCallEvent::eventName() const {
    return "StartCall";
}

void RawStartCallEvent::writeFailableEventDetailsToDebugStream(QDebug stream) const {
    stream << " initiator=" << initiator.get();
    if (peers.size() == 1) {
        stream << " peer=" << peers.front();
    } else {
        stream << " peers=" << peers;
    }
    if (durationSeconds) {
        stream << " duration_seconds=" << *durationSeconds;
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
