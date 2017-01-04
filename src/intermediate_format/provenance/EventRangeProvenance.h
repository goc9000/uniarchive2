/**
 * intermediate_format/provenance/EventRangeProvenance.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_EVENTRANGEPROVENANCE_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_EVENTRANGEPROVENANCE_H

#include "intermediate_format/provenance/Provenance.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

class EventRangeProvenance : public Provenance {
public:
    unique_ptr<Provenance> base;
    uint32_t firstEventIndex;
    uint32_t lastEventIndex;

    EventRangeProvenance(TAKE(Provenance) base, uint32_t first_event_index, uint32_t last_event_index);

    virtual ProvenanceSubType subType() const;

    virtual CEDE(Provenance) clone() const;

    static CEDE(EventRangeProvenance) deserializeFromStream(QDataStream& mut_stream, bool skip_type = false);

protected:
    virtual void serializeToStreamImpl(QDataStream &mut_stream) const;

    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_EVENTRANGEPROVENANCE_H
