/**
 * intermediate_format/events/IFCorruptedMessageEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_IFCORRUPTEDMESSAGEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_IFCORRUPTEDMESSAGEEVENT_H

#include <memory>

#include <QtDebug>
#include <QByteArray>

#include "intermediate_format/events/RawEvent.h"
#include "utils/language/shortcuts.h"

using namespace std;

namespace uniarchive2 { namespace intermediate_format { namespace events {

struct IFCorruptedMessageEvent : RawEvent {
public:
    QByteArray rawData;

    IFCorruptedMessageEvent(IMM(ApparentTime) timestamp, unsigned int index, IMM(QByteArray) raw_data);

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_IFCORRUPTEDMESSAGEEVENT_H
