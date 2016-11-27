/**
 * intermediate_format/events/RawEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 5dd2f0f9f8c104e5cb6a41b129279ac342c9c56bafefe130cd6850643b633d5e
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawEvent.h"
#include "utils/qt/shortcuts.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawEvent::RawEvent(IMM(ApparentTime) timestamp, uint32_t index)
  : StandardPolymorphic(), timestamp(timestamp), indexInConversation(index) {
}

QString RawEvent::eventName() const {
    return name_for_raw_event_sub_type(subType());
}

void RawEvent::serializeToStreamImpl(QDataStream& mut_stream) const {
    // TODO: add field serialization code

    serializeDetailsToStream(mut_stream);
}

void RawEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    // Override this in children
}

void RawEvent::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace();

    stream << "#" << indexInConversation << " ";
    stream << "[" << timestamp << "] ";

    stream << QP(eventName());
    writeDetailsToDebugStream(stream);

    if (skypeGUID) {
        stream << " skype_guid=" << *skypeGUID;
    }
}

void RawEvent::writeDetailsToDebugStream(QDebug UNUSED stream) const {
}

}}}
