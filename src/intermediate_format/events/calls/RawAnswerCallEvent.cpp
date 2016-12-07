/**
 * intermediate_format/events/calls/RawAnswerCallEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 6dc07ece14f01e1dd84871ec1539bab8dd9f99204212857bfd30af493dd4eb1e
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/calls/RawAnswerCallEvent.h"
#include "utils/qt/debug_extras.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawAnswerCallEvent::RawAnswerCallEvent(IMM(ApparentTime) timestamp, uint32_t index, TAKE(ApparentSubject) receiver)
  : RawEvent(timestamp, index), receiver(move(receiver)) {
    sanityCheckMandatoryParameters();
}

void RawAnswerCallEvent::sanityCheckMandatoryParameters() const {
    invariant(receiver, "Parameter 'receiver' cannot have empty value");
}

RawEventSubType RawAnswerCallEvent::subType() const {
    return RawEventSubType::ANSWER_CALL;
}

void RawAnswerCallEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    // TODO: add field serialization code
}

void RawAnswerCallEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " receiver=" << receiver;
    if (caller) {
        stream << " caller=" << caller;
    }
}

}}}
