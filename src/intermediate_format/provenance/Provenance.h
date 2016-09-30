/**
 * intermediate_format/provenance/Provenance.h
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#ifndef UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_PROVENANCE_H
#define UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_PROVENANCE_H

#include "intermediate_format/provenance/ProvenanceSubType.h"
#include "utils/polymorphics/StandardPolymorphic.h"
#include "utils/language/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

using namespace uniarchive2::utils::polymorphics;

class Provenance : public StandardPolymorphic<ProvenanceSubType> {
public:
    virtual CEDE(Provenance) clone() const = 0;

    void writeToDebugStream(QDebug stream) const;

protected:
    virtual void serializeToStreamImpl(QDataStream& mut_stream) const = 0;

    virtual void writeToDebugStreamImpl(QDebug stream) const = 0;
};

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_PROVENANCE_H
