/**
 * intermediate_format/events/RawEditedPreviousMessageEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 2941c848c81ccf4d222106d30091c4383d6e110860c3270f26026dcfa650d6da
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/RawEditedPreviousMessageEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawEditedPreviousMessageEvent::RawEditedPreviousMessageEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) subject
) : RawEvent(timestamp, index), subject(move(subject)) {
    sanityCheckMandatoryParameters();
}

void RawEditedPreviousMessageEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
}

RawEventSubType RawEditedPreviousMessageEvent::subType() const {
    return RawEventSubType::EDITED_PREVIOUS_MESSAGE;
}

void RawEditedPreviousMessageEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    // TODO: add field serialization code
}

void RawEditedPreviousMessageEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
}

}}}
