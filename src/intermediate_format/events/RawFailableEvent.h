/**
 * intermediate_format/events/RawFailableEvent.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWFAILABLEEVENT_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWFAILABLEEVENT_H

#include "intermediate_format/events/RawEvent.h"
#include "utils/external_libs/optional.hpp"
#include "utils/language/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace events {

using namespace std;
using namespace std::experimental;

template<typename TReason>
struct RawFailableEvent : RawEvent {
public:
    optional<TReason> reasonFailed;

    RawFailableEvent(IMM(ApparentTime) timestamp, unsigned int index) : RawEvent(timestamp, index) {}

protected:
    void writeDetailsToDebugStream(QDebug stream) const {
        if (reasonFailed) {
            stream << " FAILED";
            if ((int)*reasonFailed != 0) {
                stream << "(" << *reasonFailed << ")";
            }
        }

        writeFailableEventDetailsToDebugStream(stream);
    }

    virtual void writeFailableEventDetailsToDebugStream(QDebug UNUSED stream) const {
    }
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_EVENTS_RAWFAILABLEEVENT_H
