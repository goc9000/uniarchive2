/**
 * intermediate_format/provenance/Provenance.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/provenance/Provenance.h"
#include "intermediate_format/provenance/FileProvenance.h"
#include "intermediate_format/provenance/ArchiveFileProvenance.h"
#include "intermediate_format/provenance/AdiumArchiveFileProvenance.h"
#include "intermediate_format/provenance/EventRangeProvenance.h"
#include "intermediate_format/provenance/MSNConversationProvenance.h"
#include "intermediate_format/provenance/SkypeConversationProvenance.h"
#include "utils/serialization/deserialization_helpers.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

CEDE(Provenance) Provenance::deserializeFromStream(QDataStream& mut_stream) {
    ProvenanceSubType subtype = must_deserialize(mut_stream, ProvenanceSubType);

    switch (subtype) {
        case ProvenanceSubType::FILE:
            return FileProvenance::deserializeFromStream(mut_stream, true);
        case ProvenanceSubType::ARCHIVE_FILE:
            return ArchiveFileProvenance::deserializeFromStream(mut_stream, true);
        case ProvenanceSubType::ADIUM_ARCHIVE_FILE:
            return AdiumArchiveFileProvenance::deserializeFromStream(mut_stream, true);
        case ProvenanceSubType::EVENT_RANGE:
            return EventRangeProvenance::deserializeFromStream(mut_stream, true);
        case ProvenanceSubType::MSN_CONVERSATION:
            return MSNConversationProvenance::deserializeFromStream(mut_stream, true);
        case ProvenanceSubType::SKYPE_CONVERSATION:
            return SkypeConversationProvenance::deserializeFromStream(mut_stream, true);
    }

    invariant_violation("Invalid deserialized Provenance subtype (code: %d)", (int)subtype);
}

void Provenance::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace();

    writeToDebugStreamImpl(stream);
}

}}}
