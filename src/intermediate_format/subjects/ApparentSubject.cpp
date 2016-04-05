/**
 * intermediate_format/subjects/ApparentSubject.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include <QDebugStateSaver>

#include "intermediate_format/subjects/ApparentSubject.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

QDebug operator<< (QDebug stream, ApparentSubject const * const subject) {
    subject->writeToDebugStream(stream);
    return stream;
}

}}}
