/**
 * intermediate_format/events/conference/RawChangeConferencePictureEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/events/conference/RawChangeConferencePictureEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawChangeConferencePictureEvent::RawChangeConferencePictureEvent(
    IMM(ApparentTime) timestamp,
    uint index,
    TAKE(ApparentSubject) subject
): RawEvent(timestamp, index), subject(move(subject)) {
}

QString RawChangeConferencePictureEvent::eventName() const {
    return "ChangeConferencePicture";
}

void RawChangeConferencePictureEvent::writeDetailsToDebugStream(QDebug stream) const {
    stream << " subject=" << subject.get();
}

}}}
