/**
 * intermediate_format/events/calls/RawStartCallEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTARTCALLEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTARTCALLEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/errors/StartCallFailReason.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace std;
using namespace std::experimental;
using namespace uniarchive2::intermediate_format::errors;
using namespace uniarchive2::intermediate_format::subjects;

struct RawStartCallEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> initiator;
    vector<unique_ptr<ApparentSubject>> peers;

    optional<unsigned int> durationSeconds;

    optional<StartCallFailReason> reasonFailed;

    optional<QString> skypeCallGUID;
    optional<QString> syntheticCallGUID;
    optional<uint64_t> correspondingSkypeCallID;

    RawStartCallEvent(
        IMM(ApparentTime) timestamp,
        unsigned int index,
        TAKE(ApparentSubject) initiator,
        TAKE_VEC(ApparentSubject) peers
    );

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWSTARTCALLEVENT_H
