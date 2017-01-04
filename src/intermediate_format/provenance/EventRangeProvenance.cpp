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
#include "utils/serialization/deserialization_helpers.h"
#include "utils/serialization/serialization_helpers.h"

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

EventRangeProvenance::EventRangeProvenance(
    TAKE(Provenance) base,
    uint32_t first_event_index,
    uint32_t last_event_index
) : base(move(base)), firstEventIndex(first_event_index), lastEventIndex(last_event_index) {
}

ProvenanceSubType EventRangeProvenance::subType() const {
    return ProvenanceSubType::EVENT_RANGE;
}

CEDE(Provenance) EventRangeProvenance::clone() const {
    return make_unique<EventRangeProvenance>(base->clone(), firstEventIndex, lastEventIndex);
}

CEDE(EventRangeProvenance) EventRangeProvenance::deserializeFromStream(QDataStream& mut_stream, bool skip_type) {
    maybeDeserializeType(skip_type, mut_stream, ProvenanceSubType::EVENT_RANGE);

    return make_unique<EventRangeProvenance>(
        must_deserialize(mut_stream, unique_ptr<Provenance>),
        must_deserialize(mut_stream, uint32_t),
        must_deserialize(mut_stream, uint32_t)
    );
}

void EventRangeProvenance::serializeToStreamImpl(QDataStream& mut_stream) const {
    mut_stream << base << firstEventIndex << lastEventIndex;
}

void EventRangeProvenance::writeToDebugStreamImpl(QDebug stream) const {
    stream << "EventRange(from=" << base << ", first_event_index=" << firstEventIndex;
    stream << ", last_event_index=" << lastEventIndex << ")";
}

}}}
