/**
 * intermediate_format/events/conference/RawChangeConferencePictureEvent.cpp
 *
 * WARNING: This file is autogenerated. To make changes, edit autogen_config.yml instead and re-generate.
 * Codegen hash (sha256): 9e9a0fabdeaa641c1a7d144a1ebe457493da4cf962466106c165d956d1a1f87f
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawChangeConferencePictureEvent.h"
#include "utils/qt/debug_extras.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawChangeConferencePictureEvent::RawChangeConferencePictureEvent(
    IMM(ApparentTime) timestamp,
    uint32_t index,
    TAKE(ApparentSubject) subject
) : RawEvent(timestamp, index), subject(move(subject)) {
    sanityCheckMandatoryParameters();
}

void RawChangeConferencePictureEvent::sanityCheckMandatoryParameters() const {
    invariant(subject, "Parameter 'subject' cannot have empty value");
}

RawEventSubType RawChangeConferencePictureEvent::subType() const {
    return RawEventSubType::CHANGE_CONFERENCE_PICTURE;
}

void RawChangeConferencePictureEvent::serializeDetailsToStream(QDataStream& mut_stream) const {
    mut_stream << subject;
}

void RawChangeConferencePictureEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject;
}

}}}
