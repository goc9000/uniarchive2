/**
 * intermediate_format/events/RawCorruptedMessageEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCORRUPTEDMESSAGEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCORRUPTEDMESSAGEEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "utils/language/shortcuts.h"

#include <QtDebug>
#include <QByteArray>

#include <memory>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace std;

struct RawCorruptedMessageEvent : RawEvent {
public:
    QByteArray rawData;

    RawCorruptedMessageEvent(IMM(ApparentTime) timestamp, unsigned int index, IMM(QByteArray) raw_data);

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWCORRUPTEDMESSAGEEVENT_H
