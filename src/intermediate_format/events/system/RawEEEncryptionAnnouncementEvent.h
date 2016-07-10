/**
 * intermediate_format/events/system/RawEEEncryptionAnnouncementEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEEENCRYPTIONANNOUNCEMENTEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEEENCRYPTIONANNOUNCEMENTEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "utils/language/shortcuts.h"

#include <QtDebug>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace std;

struct RawEEEncryptionAnnouncementEvent : RawEvent {
public:
    RawEEEncryptionAnnouncementEvent(IMM(ApparentTime) timestamp, unsigned int index);

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEEENCRYPTIONANNOUNCEMENTEVENT_H
