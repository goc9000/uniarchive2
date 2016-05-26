/**
 * intermediate_format/events/RawMessageSendFailedEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWMESSAGESENDFAILEDEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWMESSAGESENDFAILEDEVENT_H

#include <memory>

#include <QtDebug>

#include "utils/external_libs/optional.hpp"

#include "intermediate_format/content/RawMessageContent.h"
#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/language/shortcuts.h"

using namespace std;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::subjects;

namespace uniarchive2 { namespace intermediate_format { namespace events {

struct RawMessageSendFailedEvent : RawEvent {
public:
    enum class SendFailReason {
        UNKNOWN = 0,
        MESSAGE_TOO_LARGE,
        RECIPIENT_OFFLINE,
        CONNECTION_ERROR
    };

    unique_ptr<ApparentSubject> sender;
    unique_ptr<ApparentSubject> receiver;
    SendFailReason reasonFailed;
    RawMessageContent unsentMessageContent;

    RawMessageSendFailedEvent(
        IMM(ApparentTime) timestamp,
        unsigned int index,
        TAKE(ApparentSubject) sender,
        SendFailReason reason,
        RawMessageContent&& unsentMessageContent = RawMessageContent()
    );

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWMESSAGESENDFAILEDEVENT_H
