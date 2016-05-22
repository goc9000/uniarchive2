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
#include "utils/qt/shortcuts.h"

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

ApparentSubject::ApparentSubject(Hints hints) : hints(hints) {
}

QDebug operator<< (QDebug stream, ApparentSubject const * const subject) {
    QDebugStateSaver save(stream);
    stream.nospace();
    subject->writeToDebugStream(stream);

    if (subject->hints) {
        QStringList hints_repr;

        if (subject->hints.testFlag(ApparentSubject::Hint::IsIdentity)) {
            hints_repr << "ident";
        }
        if (subject->hints.testFlag(ApparentSubject::Hint::IsPeer)) {
            hints_repr << "peer";
        }

        stream << " [" << QP(hints_repr.join(',')) << "]";
    }

    return stream;
}

}}}
