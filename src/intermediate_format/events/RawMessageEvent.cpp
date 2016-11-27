/**
 * intermediate_format/events/RawMessageEvent.cpp
 *
 * WARNING: This file is partially autogenerated. Edit only the custom sections, or autogen_config.yml and re-generate.
 * Codegen hash (sha256): 5f73d190a197e98f6f6884e0a5f19267ba39ee99b4948223adceb1fa459b1d0a
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawMessageEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawMessageEvent::RawMessageEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) sender,
    RawMessageContent&& content
) : RawFailableEvent(timestamp, index), sender(move(sender)), content(move(content)) {
    sanityCheckMandatoryParameters();
}

void RawMessageEvent::sanityCheckMandatoryParameters() const {
    invariant(sender, "Parameter 'sender' cannot have empty value");
}

RawEventSubType RawMessageEvent::subType() const {
    return RawEventSubType::MESSAGE;
}

void RawMessageEvent::serializeFailableEventDetailsToStream(QDataStream& mut_stream) const {
    // TODO: add field serialization code
}

void RawMessageEvent::writeFailableEventDetailsToDebugStream(QDebug stream) const {
    // BEGIN CUSTOM SECTION: Debug write method
    if (isOffline) {
        stream << " OFFLINE";
    }
    if (isEdited) {
        stream << " EDITED";
        if (editedBy || timeEdited) {
            stream << "(";
            if (editedBy) {
                stream << "edited_by=" << editedBy.get();
            }
            if (timeEdited) {
                stream << (editedBy ? " " : "") << "time_edited=" << *timeEdited;
            }
            stream << ")";
        }
    }
    if (isAction) {
        stream << " ACTION";
    }
    stream << " sender=" << sender.get();
    if (receiver) {
        stream << " receiver=" << receiver.get();
    }
    stream << " content=" << content;
    // END CUSTOM SECTION: Debug write method
}

}}}
