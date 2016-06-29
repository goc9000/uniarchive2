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

#include "utils/language/shortcuts.h"

#include <QtDebug>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

using namespace std;

class Provenance {
public:
    virtual CEDE(Provenance) clone() const = 0;

    virtual void writeToDebugStream(QDebug stream) const = 0;
};

QDebug operator<< (QDebug stream, CPTR(Provenance) provenance);

}}}

#endif //UNIARCHIVE2_INTERMEDIATE_FORMAT_PROVENANCE_PROVENANCE_H
