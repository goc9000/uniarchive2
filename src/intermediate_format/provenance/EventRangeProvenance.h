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
    uint firstEventIndex;
    uint lastEventIndex;

    EventRangeProvenance(TAKE(Provenance) base, uint first_event_index, uint last_event_index);

    virtual CEDE(Provenance) clone() const;

protected:
    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_EVENTRANGEPROVENANCE_H
