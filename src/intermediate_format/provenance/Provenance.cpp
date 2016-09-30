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

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace provenance {

void Provenance::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver saver(stream);
    stream.nospace();

    writeToDebugStreamImpl(stream);
}

}}}
