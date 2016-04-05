/**
 * intermediate_format/events/IntermediateFormatEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_INTERMEDIATEFORMATEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_INTERMEDIATEFORMATEVENT_H

#include <QtDebug>
#include <QString>

#include "intermediate_format/ApparentTime.h"

namespace uniarchive2 { namespace intermediate_format { namespace events {

struct IntermediateFormatEvent {
public:
    ApparentTime timestamp;
    unsigned int indexInConversation;

    IntermediateFormatEvent(ApparentTime timestamp, unsigned int index);

    virtual QString eventName() const = 0;
    void writeToDebugStream(QDebug stream) const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

QDebug operator<< (QDebug stream, IntermediateFormatEvent const * const event);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_INTERMEDIATEFORMATEVENT_H
