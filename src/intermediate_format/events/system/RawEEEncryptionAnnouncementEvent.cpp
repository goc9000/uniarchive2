/**
 * intermediate_format/events/system/RawEEEncryptionAnnouncementEvent.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/events/system/RawEEEncryptionAnnouncementEvent.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

RawEEEncryptionAnnouncementEvent::RawEEEncryptionAnnouncementEvent(IMM(ApparentTime) timestamp, unsigned int index)
    : RawEvent(timestamp, index) {
}

QString RawEEEncryptionAnnouncementEvent::eventName() const {
    return "EEEncryptionAnnouncement";
}

void RawEEEncryptionAnnouncementEvent::writeDetailsToDebugStream(QDebug stream) const {
}

}}}
