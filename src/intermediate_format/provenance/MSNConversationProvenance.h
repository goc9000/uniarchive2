/**
 * intermediate_format/provenance/MSNConversationProvenance.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_MSNCONVERSATIONPROVENANCE_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_MSNCONVERSATIONPROVENANCE_H

#include "intermediate_format/provenance/Provenance.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

class MSNConversationProvenance : public Provenance {
public:
    unique_ptr<Provenance> base;
    uint32_t sessionID;

    MSNConversationProvenance(TAKE(Provenance) base, uint32_t session_id);

    virtual ProvenanceSubType subType() const;

    virtual CEDE(Provenance) clone() const;

    static CEDE(MSNConversationProvenance) deserializeFromStream(QDataStream& mut_stream, bool skip_type = false);

protected:
    virtual void serializeToStreamImpl(QDataStream &mut_stream) const;

    virtual void writeToDebugStreamImpl(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_MSNCONVERSATIONPROVENANCE_H
