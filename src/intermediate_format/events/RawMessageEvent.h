/**
 * intermediate_format/events/RawMessageEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWMESSAGEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWMESSAGEEVENT_H

#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace std;
using namespace std::experimental;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::subjects;

struct RawMessageEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> sender;
    unique_ptr<ApparentSubject> receiver;
    RawMessageContent content;

    bool isOffline = false;

    bool isEdited = false;
    unique_ptr<ApparentSubject> editedBy;
    ApparentTime timeEdited;

    RawMessageEvent(
        IMM(ApparentTime) timestamp,
        unsigned int index,
        TAKE(ApparentSubject) sender,
        RawMessageContent&& content
    );

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWMESSAGEEVENT_H
