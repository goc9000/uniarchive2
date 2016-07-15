/**
 * intermediate_format/events/RawEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEVENT_H

#include "intermediate_format/ApparentTime.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/polymorphic_helpers.h"
#include "utils/language/shortcuts.h"

#include <QtDebug>
#include <QByteArray>
#include <QString>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace std::experimental;

struct RawEvent {
public:
    ApparentTime timestamp;
    unsigned int indexInConversation;

    optional<QByteArray> skypeGUID;

    RawEvent(IMM(ApparentTime) timestamp, unsigned int index);

    POLYMORPHIC_HELPERS

    virtual QString eventName() const = 0;
    void writeToDebugStream(QDebug stream) const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

QDebug operator<< (QDebug stream, CPTR(RawEvent) event);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWEVENT_H
