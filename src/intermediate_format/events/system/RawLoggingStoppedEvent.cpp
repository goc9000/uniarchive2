/**
 * intermediate_format/events/system/RawLoggingStoppedEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 3438aec619277954810d89cfeb8c16852978487b3aafb4b10fb7cf3eef968538
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/system/RawLoggingStoppedEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawLoggingStoppedEvent::RawLoggingStoppedEvent(IMM(ApparentTime) timestamp, uint index) : RawEvent(timestamp, index) {
}

RawEventSubType RawLoggingStoppedEvent::subType() const {
    return RawEventSubType::LOGGING_STOPPED;
}

void RawLoggingStoppedEvent::serializeDetailsToStream(QDataStream& UNUSED mut_stream) const {
    // TODO: add field serialization code
}

void RawLoggingStoppedEvent::writeDetailsToDebugStream(QDebug UNUSED stream) const {
}

}}}
