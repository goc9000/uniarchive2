/**
 * intermediate_format/events/status/RawStatusChangeEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTATUSCHANGEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTATUSCHANGEEVENT_H

#include <QtDebug>

#include "utils/external_libs/optional.hpp"

#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "protocols/IMStatus.h"
#include "utils/language/shortcuts.h"

using namespace std;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::subjects;
using namespace uniarchive2::protocols;

namespace uniarchive2 { namespace intermediate_format { namespace events {

struct RawStatusChangeEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> subject;
    IMStatus status;
    optional<RawMessageContent> message;

    RawStatusChangeEvent(
        IMM(ApparentTime) timestamp,
        unsigned int index,
        TAKE(ApparentSubject) subject,
        IMStatus status
    );

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTATUSCHANGEEVENT_H
