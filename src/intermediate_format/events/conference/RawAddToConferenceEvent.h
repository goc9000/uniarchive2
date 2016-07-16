/**
 * intermediate_format/events/conference/RawAddToConferenceEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWADDTOCONFERENCEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWADDTOCONFERENCEEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <QtDebug>

#include <vector>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace std;
using namespace std::experimental;
using namespace uniarchive2::intermediate_format::subjects;

struct RawAddToConferenceEvent : RawEvent {
public:
    unique_ptr<ApparentSubject> adder;
    vector<unique_ptr<ApparentSubject>> addees;

    optional<bool> asModerator;

    RawAddToConferenceEvent(
        IMM(ApparentTime) timestamp,
        unsigned int index,
        TAKE(ApparentSubject) adder,
        TAKE_VEC(ApparentSubject) addees
    );
    RawAddToConferenceEvent(
        IMM(ApparentTime) timestamp,
        unsigned int index,
        TAKE(ApparentSubject) adder,
        TAKE(ApparentSubject) addee
    );
    RawAddToConferenceEvent(
        IMM(ApparentTime) timestamp,
        unsigned int index,
        TAKE(ApparentSubject) addee
    );

    virtual QString eventName() const;

protected:
    virtual void writeDetailsToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWADDTOCONFERENCEEVENT_H
