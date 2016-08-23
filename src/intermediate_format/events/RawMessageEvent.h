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
#include "intermediate_format/events/RawFailableEvent.h"
#include "intermediate_format/errors/SendMessageFailReason.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace std;
using namespace std::experimental;
using namespace uniarchive2::intermediate_format::content;
using namespace uniarchive2::intermediate_format::errors;
using namespace uniarchive2::intermediate_format::subjects;

struct RawMessageEvent : RawFailableEvent<SendMessageFailReason> {
public:
    unique_ptr<ApparentSubject> sender;
    unique_ptr<ApparentSubject> receiver;
    RawMessageContent content;

    bool isOffline = false;
    /**
     * Action messages represent a supposed action or status of the subject, as opposed to the verbatim content of an
     * utterance. Example: <subject> *runs and hides*
     */
    bool isAction = false;

    bool isEdited = false;
    unique_ptr<ApparentSubject> editedBy;
    ApparentTime timeEdited;

    RawMessageEvent(IMM(ApparentTime) timestamp, uint index, TAKE(ApparentSubject) sender, RawMessageContent&& content);

    virtual QString eventName() const;

protected:
    virtual void writeFailableEventDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWMESSAGEEVENT_H
