/**
 * intermediate_format/subjects/ApparentSubject.cpp
 *
 * (C) Copyright 2014-present  Cristian Dinu <goc9000@gmail.com>
 *
 * This file is part of UniArchive II.
 *
 * Licensed under the GPL-3
 */

#include "intermediate_format/subjects/ApparentSubject.h"
#include "utils/qt/shortcuts.h"

#include <QDebugStateSaver>

namespace uniarchive2 { namespace intermediate_format { namespace subjects {

ApparentSubject::ApparentSubject(Hints hints) : hints(hints) {
}

void ApparentSubject::serializeToStreamImpl(QDataStream &mut_stream) const {
    mut_stream << hints;
    serializeToStreamSubImpl(mut_stream);
}

void ApparentSubject::writeToDebugStream(QDebug stream) const {
    QDebugStateSaver save(stream);
    stream.nospace();
    writeToDebugStreamImpl(stream);

    if (hints) {
        QStringList hints_repr;

        if (hints.testFlag(ApparentSubject::Hint::IsIdentity)) {
            hints_repr << "ident";
        }
        if (hints.testFlag(ApparentSubject::Hint::IsPeer)) {
            hints_repr << "peer";
        }

        stream << " [" << QP(hints_repr.join(',')) << "]";
    }
}

}}}
