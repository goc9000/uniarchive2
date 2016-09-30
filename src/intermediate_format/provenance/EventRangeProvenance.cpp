/**
 * intermediate_format/provenance/EventRangeProvenance.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/provenance/EventRangeProvenance.h"
#include "utils/qt/debug_extras.h"

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

EventRangeProvenance::EventRangeProvenance(TAKE(Provenance) base, uint first_event_index, uint last_event_index)
    : base(move(base)), firstEventIndex(first_event_index), lastEventIndex(last_event_index) {
}

CEDE(Provenance) EventRangeProvenance::clone() const {
    return make_unique<EventRangeProvenance>(base->clone(), firstEventIndex, lastEventIndex);
}

void EventRangeProvenance::writeToDebugStreamImpl(QDebug stream) const {
    stream << "EventRange(from=" << base << ", first_event_index=" << firstEventIndex;
    stream << ", last_event_index=" << lastEventIndex << ")";
}

}}}
