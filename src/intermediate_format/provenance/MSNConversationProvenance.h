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

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

using namespace std;

class MSNConversationProvenance : public Provenance {
public:
    unique_ptr<Provenance> base;
    unsigned int sessionID;

    MSNConversationProvenance(TAKE(Provenance) base, unsigned int session_id);

    virtual CEDE(Provenance) clone() const;

    virtual void writeToDebugStream(QDebug stream) const;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_MSNCONVERSATIONPROVENANCE_H
