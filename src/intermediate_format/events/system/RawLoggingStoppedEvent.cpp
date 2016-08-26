/**
 * intermediate_format/events/system/RawLoggingStoppedEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 915b8dee166133a0094e0cb9fc573a4d7b3f7dae043580b35875342b4903fbeb
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

QString RawLoggingStoppedEvent::eventName() const {
    return "LoggingStopped";
}

void RawLoggingStoppedEvent::writeDetailsToDebugStream(QDebug UNUSED stream) const {
}

}}}
